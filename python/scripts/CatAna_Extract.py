#!/usr/bin/env python
from __future__ import division, print_function, absolute_import

import CatAna.io as io

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


def extract(infile, outfile, intype, outtype, max_dist, precision='float',
            incoord='cartesian', incoord_unit='Mpc', outcoord='cartesian', outcoord_unit='Mpc',
            hubble_param=1., box_origin=0., filter = [], subsample_size=None, temp_file=None, verbose=True):

        #  Prepare Sink
        assert outcoord_unit in ['Mpc', 'Mpc/h']
        if outcoord_unit == 'Mpc':
            output_hubble_param = 1.
        else:
            output_hubble_param = hubble_param
        pysink = io.PySink(outfile, outtype, precision=precision, coord=outcoord,
                           hubble_param=output_hubble_param, box_origin=0)

        #  Prepare Source
        assert incoord_unit in ['Mpc', 'Mpc/h']
        if incoord_unit == 'Mpc':
            input_hubble_param = 1.
        else:
            input_hubble_param = hubble_param
        pysource = io.PySource(infile, intype, verbose, coord=incoord, hubble_param=input_hubble_param, box_origin=box_origin)

        pyfilterstream = io.PyFilterStream(pysource, pysink, subsample_size, temp_file, verbose)

        tophat_defined = False
        if filter is not None:
            if not hasattr(filter, '__len__'):
                filters = [filter]
            tophat_defined = any([f.filter == "tophat" for f in filter])

        if not tophat_defined:
            pyfilterstream.add_filter(io.PyFilter("tophat", max_dist))
        if filter is not None:
            for f in filter:
                pyfilterstream.add_filter(io.PyFilter(f.filter, f.option, rmax=max_dist))

        pyfilterstream.run()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
            description="Extract and filter particle positions from gadget/text files")
    parser.add_argument("infile", type=str,
                        help="the input file", )
    parser.add_argument("outfile", type=str,
                        help="the output file")
    parser.add_argument("--intype", type=str, choices=["Gadget", "Text"],
                        help="type of input file (may be deduced from filename")
    parser.add_argument("--outtype", type=str, choices=["Text"],
                        help="type of output file (may be deduced from filename")
    parser.add_argument("--precision", type=str, choices=["float", "double"], default='float',
                        help="Data precision in input file")
    parser.add_argument("--incoord", type=str, choices=["cartesian", "spherical", "spherical_3dex"], default="cartesian",
                        help="Coordinate system used in the input file (only HDF5 and Text, spherical_3dex Text only)")
    parser.add_argument("--incoord_unit", type=str, choices=["Mpc", "Mpc/h"], default="Mpc")
    parser.add_argument("--outcoord", type=str, choices=["cartesian", "spherical", "spherical_3dex"], default="cartesian",
                        help="Coordinate system used for the output file (spherical_3dex text only)")
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