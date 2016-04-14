#!/usr/bin/env python

from __future__ import division, print_function, absolute_import

import CatAna.io as io
import CatAna.decomp as decomp
import argparse
import os
import collections


function_interpolation_points = 10000
supported_filters = ["tophat", "gauss", "AngMask"]
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
            try:
                getattr(args, self.dest).append(FilterTuple(values[0], option))
            except AttributeError:
                setattr(args, self.dest, [FilterTuple(values[0], option)])


def analyze(infile, lmax, nmax, outfile_base, method, intype, incoord, incoord_unit, hubble_param, max_dist,
                 box_origin, filter, survey_volume, subsample_size, verbose=True, nside=None):

        #  Prepare Source
        assert incoord_unit in ['Mpc', 'Mpc/h']
        if incoord_unit == 'Mpc':
            input_hubble_param = 1.
        else:
            input_hubble_param = hubble_param

        pysource = io.PySource(infile, intype, verbose, coord=incoord, hubble_param=input_hubble_param, box_origin=box_origin)
        analyzer = decomp.PyAnalyzer(pysource, survey_volume, subsample_size)

        tophat_defined = False
        if filter is not None:
            if not hasattr(filter, '__len__'):
                filters = [filter]
            tophat_defined = any([f.filter == "tophat" for f in filter])

        if not tophat_defined:
            analyzer.add_filter(io.PyFilter("tophat", max_dist))
        if filter is not None:
            for f in filter:
                analyzer.add_filter(io.PyFilter(f.filter, f.option, rmax=max_dist))

        if method == 'RAW':
            kclkk = analyzer.compute_sfb(lmax, nmax, max_dist, verbose)
        elif method == "REV_FFT":
            assert nside is not None
            kclkk = analyzer.compute_sfb_pixelized(lmax, nmax, max_dist, nside, verbose)
        else:
            raise RuntimeError("unknown method: {}".format(method))

        kclkk.savetxt(outfile_base)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description="Read from source file and compute C_l(k,k)")

    parser.add_argument("infile", type=str,
                        help="the input file", )
    parser.add_argument("lmax", type=int, help="Up to which l to compute C_l(k,k)")
    parser.add_argument("nmax", type=int, help="Number of k's for each l to compute")
    parser.add_argument("outfile_base", type=str,
                        help="filename base of output files")
    parser.add_argument("--method", type=str, choices=["RAW", "REV_FFT"], default="REV_FFT",
                        help="method to use for C_l(k,k) computation")
    parser.add_argument("--nside", type=int, default=256,
                        help="NSide for REV_FFT method")
    parser.add_argument("--intype", type=str, choices=["Gadget", "Text"],
                        help="type of input file (may be deduced from filename")
    parser.add_argument("--incoord", type=str, choices=["cartesian", "spherical", "spherical_3dex"], default="cartesian",
                        help="Coordinate system used in the input file (only HDF5 and Text)")
    parser.add_argument("--incoord_unit", type=str, choices=["Mpc", "Mpc/h"], default="Mpc")
    parser.add_argument("--hubble_param", type=float, default=0.7,
                        help="If the input is in Mpc/h, use this parameter to transform coordinates to Mpc.")
    parser.add_argument("--max_dist", type=float, required=True,
                        help="The maximal distance of objects from the origin (Rmax), in input units")
    parser.add_argument("--box_origin", type=float, default=0,
                        help="Coordinate location of the box center (only cartesian coords) "
                             "(e.g. if box coordinates from [0,L] -> specify L/2)")
    parser.add_argument("--filter", nargs='*', action=ParseFilter,
                        metavar="name [scale/option]",
                        help="Filters to apply to data. Scales in input units. Valid filters: {}".format(supported_filters))
    parser.add_argument("--survey_volume", type=float, default=1,
                        help="The volume spanned by the window function of the data [in input units].")
    parser.add_argument("--subsample_size", type=int)
    parser.add_argument("--verbose", action='store_true')
    args = parser.parse_args()

    kwargs = vars(args)
    print("Arguments used:", kwargs)

    analyze(**kwargs)