from __future__ import division, print_function, absolute_import

import collections
import os

from . import io_core
io_core.init_random()

buffer_size = 100000  # Uses ca 500MB memory (measured)
function_interpolation_points = 10000  # For generic filter function

class PySource(object):
    def __init__(self, filename, filetype=None, verbose=True, **kwargs):
        if filetype is None:
            extension = filename.split('.')[-1]
            if extension in ['hdf', 'hdf5', 'h5']:
                filetype = 'HDF5'
            elif extension in ['txt','dat']:
                filetype = 'Text'
            else:
                raise RuntimeError("Could not derrive the filetype, please specify explicitly.")

        # For Text and HDF5
        hubble_param = kwargs.get('hubble_param', 1)
        box_origin = kwargs.get('box_origin', 0)
        coord = kwargs.get('coord', 'cartesian')
        assert (coord in ['cartesian', 'spherical', 'spherical_3dex'])

        if filetype == 'Gadget':
            self.source = io_core.GadgetSource(filename, verbose)

        elif filetype == 'HDF5':
            tablename = kwargs.get('tablename')
            precision = kwargs.get('precision', 'float')
            assert(precision in ['float', 'double'])

            if coord == 'cartesian':
                if precision == 'float':
                    self.source = io_core.HDF5Source_cartesian_float(filename, tablename, hubble_param, box_origin * 2, verbose)
                elif precision == 'double':
                    self.source = io_core.HDF5Source_cartesian_double(filename, tablename, hubble_param, box_origin * 2, verbose)
            elif coord == 'spherical':
                if precision == 'float':
                    self.source = io_core.HDF5Source_spherical_float(filename, tablename, hubble_param, box_origin * 2, verbose)
                elif precision == 'double':
                    self.source = io_core.HDF5Source_spherical_double(filename, tablename, hubble_param, box_origin * 2, verbose)

        elif filetype == 'Text':
            if coord == 'cartesian':
                self.source = io_core.TextSource_cartesian(filename, hubble_param, box_origin*2)
            elif coord == 'spherical':
                self.source = io_core.TextSource_spherical(filename, hubble_param, box_origin*2)
            elif coord == 'spherical_3dex':
                self.source = io_core.TextSource_spherical_3dex(filename, hubble_param, box_origin*2)

        else:
            raise RuntimeError("Unsupported filetype: {}".format(filetype))

    def get_objectcontainer(self):
        return self.source.get_objectcontainer()

    def get_pixobjectcontainer(self, nside):
        return self.source.get_pixobjectcontainer(nside)


class PySink(object):
    def __init__(self, filename, filetype="HDF5", verbose=True, **kwargs):
        if filetype is None:
            extension = filename.split('.')[-1]
            if extension in ['hdf', 'hdf5', 'h5']:
                filetype = 'HDF5'
            elif extension in ['txt','dat']:
                filetype = 'Text'
            else:
                raise RuntimeError("Could not derrive the filetype, please specify explicitly.")

        assert(filetype in ["HDF5", "Text"])

        hubble_param = kwargs.get('hubble_param', 1)
        box_origin = kwargs.get('box_origin', 0)
        coord = kwargs.get('coord', 'cartesian')
        assert (coord in ['cartesian', 'spherical', 'spherical_3dex'])

        if filetype == "HDF5":
            tablename = kwargs.get('tablename')
            precision = kwargs.get('precision', 'float')
            assert(precision in ['float', 'double'])

            if coord == 'cartesian':
                if precision == 'float':
                    self.sink = io_core.HDF5Sink_cartesian_float(filename, tablename, hubble_param, 2*box_origin, True,
                                                            verbose)
                elif precision == 'double':
                    self.sink = io_core.HDF5Sink_cartesian_double(filename, tablename, hubble_param, 2 * box_origin, True,
                                                            verbose)
            elif coord == 'spherical':
                if precision == 'float':
                    self.sink = io_core.HDF5Sink_spherical_float(filename, tablename, hubble_param, 2 * box_origin, True,
                                                            verbose)
                elif precision == 'dobule':
                    self.sink = io_core.HDF5Sink_spherical_double(filename, tablename, hubble_param, 2 * box_origin, True,
                                                            verbose)

        elif filetype == "Text":
            if coord == 'cartesian':
                self.sink = io_core.TextSink_cartesian(filename, verbose)
            elif coord == 'spherical':
                self.sink = io_core.TextSink_spherical(filename, verbose)
            elif coord == 'spherical_3dex':
                self.sink = io_core.TextSink_spherical_3dex(filename, verbose)


        else:
            raise RuntimeError("Unsupported filetype: {}".format(filetype))


class PyFilter(object):
    def __init__(self, filter_name, filter_option, **kwargs):
        assert(filter_name in ["tophat", "gauss", "generic", "AngMask"])

        if filter_name == "tophat":
            filter_option = float(filter_option)
            self.filter = io_core.TophatRadialWindowFunctionFilter(filter_option)

        elif filter_name == "gauss":
            filter_option = float(filter_option)
            self.filter = io_core.GaussianRadialWindowFunctionFilter(filter_option)

        elif filter_name == "generic":
            assert(hasattr(filter_option, "__call__"))
            rmax = kwargs.get("rmax")
            self.filter = io_core.GenericRadialWindowFunctionFilter(
                lambda r: filter_option(r), function_interpolation_points, 0, rmax)

        elif filter_name == "AngMask":
            assert(os.path.isfile(filter_option))
            self.filter = io_core.AngularMaskFilter(filter_option)


class PyFilterStream(object):
    def __init__(self, py_source, py_sink, subsample_size = None, temp_file = None, verbose=True):
        assert(isinstance(py_source, PySource))
        assert(isinstance(py_sink, PySink))
        self.source = py_source.source
        self.sink = py_sink.sink

        if subsample_size is not None:
            subsample_size = int(subsample_size)
            assert(temp_file is not None)
            self.filter_stream = io_core.FilterStream(self.source, self.sink, buffer_size, subsample_size, temp_file, verbose)
        else:
            self.filter_stream = io_core.FilterStream(self.source, self.sink, buffer_size, verbose)

        self.filters = []

    def add_filter(self, py_filter):
        assert(isinstance(py_filter, PyFilter))
        self.filters.append(py_filter.filter)  # Need to store the filter objects, otherwise destroyed
        self.filter_stream.add_filter(py_filter.filter)

    def run(self):
        self.filter_stream.run()