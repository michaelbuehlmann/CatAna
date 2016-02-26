from __future__ import division, print_function, absolute_import

import argparse
import collections
import numpy as np
import scipy.interpolate as sinterpol
import os

import PyCosmo
import CatAna.io_core as io

function_interpolation_points = 10000
valid_filters = ["tophat", "gauss", "CMASS", "AngMask"]

class GenericRedshiftWindowFunction(object):
    """
    Inherit from this class if the window function is defined in redshift space. Override the window_function
    It is assumed that the function __call__ is called with radial distances measured in Mpc (NOT Mpc/h)
    """
    def __init__(self, **cosmo_kwargs):
        """
        :param cosmo_kwargs: These arguments are passed to the PyCosmo .set function
        """
        cosmo = PyCosmo.Cosmo()
        cosmo.set(
            h=0.7,
            omega_m=0.3,
            omega_b=0.045,
            omega_l_in = 0.7,
            w0=-1.0,
            wa=0.0,
            n=1.0,
        )
        cosmo.set(**cosmo_kwargs)
        redshift2distance = lambda z: cosmo.background.dist_rad_a(1./(1+z))
        test_z = np.linspace(0,10,function_interpolation_points, endpoint=True)
        test_r = redshift2distance(test_z)
        self.redshift2distance = lambda z: sinterpol.interp1d(test_z, test_r)(z)
        self.distance2redshift = lambda r: sinterpol.interp1d(test_r, test_z)(r)

    def window_function(self, z):
        if hasattr(z, "__len__"): return np.ones_like(z)
        else: return 1.

    def __call__(self, r):  # Note: r in units Mpc
        z = self.distance2redshift(r)
        return self.window_function(z)


class CMASSWindowFunction(GenericRedshiftWindowFunction):
    def __init__(self, **cosmo_kwargs):
        super(CMASSWindowFunction, self).__init__(**cosmo_kwargs)
        self.p = np.poly1d([-850.25811069,1038.88332489,-288.1960283])
    def window_function(self, z):
        if not hasattr(z, "__len__"):
            return 0 if z>0.45 else self.p(z)*np.exp(-(z/0.291935400424)**2)
        else:
            w = np.zeros_like(z)
            w[z>0.45] = self.p(z[z>0.45])*np.exp(-(z[z>0.45]/0.291935400424)**2)
            return w


Filter = collections.namedtuple('Filter', 'filter option')
class ValidateFilter(argparse.Action):
    def __call__(self, parser, args, values, option_string=None):
        if values[0] not in valid_filters:
            parser.error("invalid filter {}".format(values[0]))
        else:
            if values[0] in ["tophat", "gauss"]:
                if len(values) == 2:
                    try:
                        R0 = float(values[1])
                    except:
                        parser.error("invalid filter scale {}".format(values[1]))
                else:
                    parser.error("need 2 arguments for filter {}".format(values[0]))
            if values[0] == "AngMask":
                if len(values) == 2:
                    if not os.path.isfile(values[1]):
                        parser.error("not a valid filename {}".format(values[1]))
                    else:
                        R0 = values[1]
                else:
                    parser.error("need to specify path to angular mask")
            if values[0] == 'CMASS':
                R0 = 0
            try:
                getattr(args, self.dest).append(Filter(values[0], R0))
            except AttributeError:
                setattr(args, self.dest, [Filter(values[0], R0)])


class ValidateFile(argparse.Action):
    def __call__(self, parser, args, values, option_string=None):
        if not os.path.exists(values):
            parser.error("{} does not exist".format(values))
        setattr(args, self.dest, values)
        extension = values.split('.')[-1]
        if extension in ['h5', 'hdf', 'hdf5']:
            if getattr(args, 'intype') is None:
                setattr(args,'intype', 'HDF5')
        elif extension in ['txt', 'dat']:
            if getattr(args, 'intype') is None:
                setattr(args,'intype', 'Text')
                setattr(args,'intype', 'Text')

def get_filter_instance(filter, max_dist):
    assert (isinstance(filter, Filter))
    if filter.filter == 'tophat':
        print("Added tophat window function with radius {}".format(filter.option))
        filter_instance = io.TophatRadialWindowFunctionFilter(filter.option)
    if filter.filter == 'gauss':
        print("Added gaussian window function with scale {}".format(filter.option))
        filter_instance = io.GaussianRadialWindowFunctionFilter(filter.option)
    if filter.filter == 'CMASS':
        print("Added CMASS window function.")
        wfct = CMASSWindowFunction()
        filter_instance = io.GenericRadialWindowFunctionFilter(
            lambda r: wfct(r), function_interpolation_points, 0, max_dist)
    if filter.filter == 'AngMask':
        print("Added Angular Mask from file {}".format(filter.option))
        ilter_instance = io.AngularMaskFilter(filter.option)


def get_source_instance(*args):
