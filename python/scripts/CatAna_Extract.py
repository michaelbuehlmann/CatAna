from __future__ import division, print_function, absolute_import

import CatAna.io as io

import argparse
import os
import collections

import numpy as np
import scipy.interpolate as sinterpol
function_interpolation_points = 10000

class GenericRedshiftWindowFunction(object):
    """
    Inherit from this class if the window function is defined in redshift space. Override the window_function
    It is assumed that the function __call__ is called with radial distances measured in Mpc (NOT Mpc/h)
    """
    def __init__(self, **cosmo_kwargs):
        """
        :param cosmo_kwargs: These arguments are passed to the PyCosmo .set function
        """
        import PyCosmo
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


supported_filters = ["tophat", "gauss", "AngMask", "CMASS"]
FilterTuple = collections.namedtuple('Filter', 'filter option')


class ParseFilter(argparse.Action):
    def __call__(self, parser, args, values, option_string=None):
        if values[0] not in supported_filters:
            parser.error("invalid filter {}".format(values[0]))
        else:
            if values[0] in ["tophat", "gauss"]:
                if len(values) == 2:
                    try:
                        option = float(values[1])
                    except:
                        parser.error("invalid filter scale {}".format(values[1]))
                else:
                    parser.error("need 2 arguments for filter {}".format(values[0]))
            if values[0] == "AngMask":
                if len(values) == 2:
                    if not os.path.isfile(values[1]):
                        parser.error("not a valid filename {}".format(values[1]))
                    else:
                        option = values[1]
                else:
                    parser.error("need to specify path to angular mask")
            if values[0] == 'CMASS':
                values[0] = 'generic'
                option = CMASSWindowFunction()
            try:
                getattr(args, self.dest).append(FilterTuple(values[0], option))
            except AttributeError:
                setattr(args, self.dest, [FilterTuple(values[0], option)])


def extract(infile, outfile, intype, outtype, max_dist, intable=None, outtable=None, precision='float',
                 incoord='cartesian', incoord_unit='Mpc', outcoord='cartesian', outcoord_unit='Mpc',
                 hubble_param=1., box_origin=0., filter = [], subsample_size=None, temp_file=None, verbose=True):

        #  Prepare Sink
        assert outcoord_unit in ['Mpc', 'Mpc/h']
        if outcoord_unit == 'Mpc':
            output_hubble_param = 1.
        else:
            output_hubble_param = hubble_param
        pysink = io.PySink(outfile, outtype, tablename=outtable, precision=precision, coord=outcoord,
                           hubble_param=output_hubble_param, box_origin=0)

        #  Prepare Source
        assert incoord_unit in ['Mpc', 'Mpc/h']
        if incoord_unit == 'Mpc':
            input_hubble_param = 1.
        else:
            input_hubble_param = hubble_param
        pysource = io.PySource(infile, intype, verbose, tablename=intable, coord=incoord, hubble_param=input_hubble_param, box_origin=box_origin)

        pyfilterstream = io.PyFilterStream(pysource, pysink, subsample_size, temp_file, verbose)

        if not hasattr(filter, '__len__'):
            filters = [filter]
        tophat_defined = any([f.filter == "tophat" for f in filter])

        if not tophat_defined:
            pyfilterstream.add_filter(io.PyFilter("tophat", max_dist))
        for f in filter:
            pyfilterstream.add_filter(io.PyFilter(f.filter, f.option, rmax=max_dist))

        pyfilterstream.run()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
            description="Extract and filter particle positions from gadget/hdf5 files")
    parser.add_argument("infile", type=str,
                        help="the input file", )
    parser.add_argument("outfile", type=str,
                        help="the output file (only HDF5 supported so far)")
    parser.add_argument("--intable", type=str,
                        help="the input table in the input file (only HDF5)")
    parser.add_argument("--outtable", type=str,
                        help="the output table in the output file")
    parser.add_argument("--intype", type=str, choices=["HDF5", "Gadget", "Text"],
                        help="type of input file (may be deduced from filename")
    parser.add_argument("--outtype", type=str, choices=["HDF5", "Text"],
                        help="type of output file (may be deduced from filename")
    parser.add_argument("--precision", type=str, choices=["float", "double"], default='float',
                        help="Data precision in input file (only HDF5)")
    parser.add_argument("--incoord", type=str, choices=["cartesian", "spherical"], default="cartesian",
                        help="Coordinate system used in the input file (only HDF5, Text)")
    parser.add_argument("--incoord_unit", type=str, choices=["Mpc", "Mpc/h"], default="Mpc")
    parser.add_argument("--outcoord", type=str, choices=["cartesian", "spherical"], default="cartesian",
                        help="Coordinate system used for the output file")
    parser.add_argument("--outcoord_unit", type=str, choices=["Mpc", "Mpc/h"], default="Mpc")
    parser.add_argument("--max_dist", type=float, required=True,
                        help="The maximal distance of objects from the origin, in input coordinates."
                             "Automatically adds tophat filter to ensure.")
    parser.add_argument("--box_origin", type=float, default=0,
                        help="Coordinate location of the box center (only cartesian coords) "
                             "(e.g. if box coordinates from [0,L] -> specify L/2)")
    parser.add_argument("--hubble_param", type=float, default=0.7,
                        help="If the input is in Mpc/h, use this parameter to transform coordinates to Mpc. Set to 1"
                             "if units are in Mpc")
    parser.add_argument("--filter", nargs='*', action=ParseFilter,
                        metavar="name [scale/option]",
                        help="Filters to apply to data. Scales in input coordinates (eg Mpc/h). Valid filters: {}".format(supported_filters))
    parser.add_argument("--subsample_size", type=int,
                        help="Size of the output catalog which is randomly subsampled after filtering")
    parser.add_argument("--temp_file", type=str,
                        help="Path to the temporary file which will be created if subsample_size is set")
    parser.add_argument("--verbose", action='store_true')
    args = parser.parse_args()

    if args.subsample_size is not None:
        assert args.temp_file is not None, "Need to specify path to a valid temporary file, if subsample_size is set."

    kwargs = vars(args)
    print("Arguments used:", kwargs)

    extract(**kwargs)