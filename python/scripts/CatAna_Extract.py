from __future__ import division, print_function, absolute_import

import CatAna.iotools as io
import argparse
import os
import collections
import numpy as np
import scipy.interpolate as sinterpol
import PyCosmo

# TODO: more generic to add sources/sinks/filters

buffer_size = 100000  # Uses ca 500MB memory (measured)
valid_filters = ["tophat", "gauss", "CMASS", "AngMask"]

class GenericRedshiftWindowFunction(object):
    """
    Inherit from this class if the window function is defined in redshift space. Override the window_function
    """
    def __init__(self, coord_hubble_param, **cosmo_kwargs):
        """
        :param coord_hubble_param: Defines the conversion to input distances (Mpc/h) to distances measured in Mpc
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
        test_z = np.linspace(0,10,20000)
        test_r = redshift2distance(test_z)*coord_hubble_param
        self.redshift2distance = lambda z: sinterpol.interp1d(test_z, test_r)(z)
        self.distance2redshift = lambda r: sinterpol.interp1d(test_r, test_z)(r)

    def window_function(self, z):
        if hasattr(z, "__len__"): return np.ones_like(z)
        else: return 1.
    def __call__(self, r):
        z = self.distance2redshift(r)
        return self.window_function(z)


class CMASSWindowFunction(GenericRedshiftWindowFunction):
    def __init__(self, coord_hubble_param, **cosmo_kwargs):
        super(CMASSWindowFunction, self).__init__(coord_hubble_param, **cosmo_kwargs)
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
        else:
            if getattr(args, 'intype') is None:
                parser.error("Unknown extension of input file. Need to specify --intype before infile.")


class Extractor(object):
    def __init__(self, infile, outfile, intype='HDF5', intable=None, outtable=None, precision='float',
                 incoord='cartesian', outcoord='cartesian', hubble_param=1., box_center=0.,
                 filter = None, subsample_size=None, verbose=True):

        #  Prepare Sink
        if outcoord == 'cartesian':
            if outtable is None:
                outtable = 'particle_pos_cartesian'
            if precision == 'float':
                self.sink = io.HDF5Sink_cartesian_float(outfile, outtable, hubble_param, 0, True, verbose)
            elif precision == 'double':
                self.sink = io.HDF5Sink_cartesian_double(outfile, outtable, hubble_param, 0, True, verbose)
            else:
                raise ValueError("unknown precision")
        elif outcoord == 'spherical':
            if outtable is None:
                outtable = 'particle_pos_spherical'
            if precision == 'float':
                self.sink = io.HDF5Sink_spherical_float(outfile, outtable, hubble_param, 0, True, verbose)
            elif precision == 'double':
                self.sink = io.HDF5Sink_spherical_double(outfile, outtable, hubble_param, 0, True, verbose)
            else:
                raise ValueError("unknown precision")
        else:
            raise ValueError("unknown coordinate system")

        #  Prepare Source
        if intype == 'HDF5':
            if incoord=='cartesian':
                if intable is None:
                    intable = 'particle_pos_cartesian'
                if precision=='float':
                    self.source = io.HDF5Source_cartesian_float(infile, intable, hubble_param, box_center*2, verbose)
                elif precision=='double':
                    self.source = io.HDF5Source_cartesian_double(infile, intable, hubble_param, box_center*2, verbose)
                else:
                    raise ValueError("unknown precision")

            elif incoord=='spherical':
                if intable is None:
                    intable = 'particle_pos_spherical'
                if precision=='float':
                    self.source = io.HDF5Source_spherical_float(infile, intable, hubble_param, box_center*2, verbose)
                elif precision=='double':
                    self.source = io.HDF5Source_spherical_double(infile, intable, hubble_param, box_center*2, verbose)
                else:
                    raise ValueError("unknown precision")
        elif intype == 'Gadget':
            self.source = io.GadgetSource(infile, verbose)
        else:
            raise ValueError("Unknown input file type")

        # Prepare filters
        self.filter_stream = io.FilterStream(self.source, self.sink, buffer_size, verbose)

        # Need to store filter instances, otherwise they are killed and FilterStream contains pointers
        # pointing to anything.
        # TODO: In FilterStream (C++), use shared_ptr for filters
        self.filter_instances = []


        if filter is not None:
            if not hasattr(filter, '__len__'):
                filter = [filter]
            for f in filter:
                assert(isinstance(f, Filter))
                if f.filter == 'tophat':
                    print("Added tophat window function with radius {}".format(f.option))
                    self.filter_instances.append(io.TophatRadialWindowFunctionFilter(f.option))
                if f.filter == 'gauss':
                    print("Added gaussian window function with scale {}".format(f.option))
                    self.filter_instances.append(io.GaussianRadialWindowFunctionFilter(f.option))
                if f.filter == 'CMASS':
                    print("Added CMASS window function. Coordinates -> Mpc: /({})".format(hubble_param))
                    wfct = CMASSWindowFunction(hubble_param)
                    self.filter_instances.append(io.GenericRadialWindowFunctionFilter(lambda r: wfct(r)))
                if f.filter == 'AngMask':
                    print("Added Angular Mask from file {}".format(f.option))
                    self.filter_instances.append(io.AngularMaskFilter(f.option))

        for f in self.filter_instances:
            self.filter_stream.add_filter(f)

        if subsample_size is not None:
            raise NotImplementedError

        self.initialized = True

    def run(self):
        if self.initialized:
            n = self.filter_stream.run()
            del self.filter_stream  # This closes the opened files
            self.initialized = False
        else:
            raise Exception("Extractor has already run. Please reinitialize this class.")



if __name__ == "__main__":
    parser = argparse.ArgumentParser(
            description="Extract and filter particle positions from gadget/hdf5 files")
    parser.add_argument("infile", type=str, action=ValidateFile,
                        help="the input file", )
    parser.add_argument("outfile", type=str,
                        help="the output file (only HDF5 supported so far)")
    parser.add_argument("--intable", type=str,
                        help="the input table in the input file (only HDF5)")
    parser.add_argument("--outtable", type=str,
                        help="the output table in the output file")
    parser.add_argument("--intype", type=str, choices=["HDF5", "Gadget"],
                        help="type of input file (may be deduced from filename")
    parser.add_argument("--precision", type=str, choices=["float", "double"], default='float',
                        help="Data precision in input file (only HDF5)")
    parser.add_argument("--incoord", type=str, choices=["cartesian", "spherical"], default="cartesian",
                        help="Coordinate system used in the input file (only HDF5)")
    parser.add_argument("--outcoord", type=str, choices=["cartesian", "spherical"], default="cartesian",
                        help="Coordinate system used for the output file")
    parser.add_argument("--box_center", type=float, default=0,
                        help="If the input coordinates are cartesian in range [0, L], this will tell the program "
                             "where the center is (i.e. specify L/2)")
    parser.add_argument("--hubble_param", type=float, default=1,
                        help="If the input is in Mpc/h, use this parameter to transform coordinates to Mpc. Set to 1"
                             "if units are in Mpc")
    parser.add_argument("--filter", nargs='*', action=ValidateFilter,
                        metavar="filtername {} [scale]".format(valid_filters),
                        help="Filters to apply to data")
    parser.add_argument("--subsample_size", type=int,
                        help="Size of the output catalog which is randomly subsampled after filtering")
    parser.add_argument("--verbose", action='store_true')

    args = parser.parse_args()
    kwargs = vars(args)
    print(kwargs)

    ex = Extractor(**kwargs)
    ex.run()