"""
This module is a convenience wrapper of the CatAna.io_core python module.

The CatAna.io_core module directly exposes the CatAna "io" library functions written in C++. This module is a
convenience wrapper around the CatAna.io_core module for simplified usage.
"""

from __future__ import division, print_function, absolute_import

import collections
import healpy as hp
import os

from . import basictypes
from . import io_core
io_core.init_random()

## The number of objects which are loaded in chunck from the Source functions. Change if memory consumption too high.
buffer_size = 1000000

## The number of interpolation points used to interpolate the function used for the "generic" radial window function filter.
function_interpolation_points = 10000  # For generic filter function


class PySource(object):
    """A manager for different sources of position data of the particles.

    Currently supported are:
    - Text: space delimited columns of either cartesian coordinates (x, y, z) or spherical coordinates (r, theta phi)
      or (phi, theta, r)
    - Gadget snapshot files
    """

    def __init__(self, filename, filetype=None, verbose=True, **kwargs):
        """
        @param filename The filename of the Source file. For multiple Gadget files belonging to same snapshot, pick any.
        @param filetype The type of source. Either "Text" or "Gadget"
        @param verbose verbosity

        For Text sources, you can further specify:
        - coord: either 'cartesian', 'spherical' (r, theta, phi) or 'spherical_3dex' (phi, theta, r)
        - hubble_param: If coordinates are in Mpc/h, use this parameter to convert to Mpc
        - box_origin: If cartesian coordinates, specify the "center of the box"
        """
        if filetype is None:
            extension = filename.split('.')[-1]
            if extension in ['txt','dat']:
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
        """ Retreive an instance of ObjectContainer
        Loads all data from the source and returns an ObjectContainer.
        """
        return self.source.get_objectcontainer()

    def get_pixobjectcontainer(self, nside):
        """ Retreive an instance of PixelizedObjectContainer
        Loads all data from the source and returns an PixelizedObjectContainer.
        """
        return self.source.get_pixobjectcontainer(nside)

    def reset(self):
        """ Resets source to initial state
        """
        self.source.reset()


class PySink(object):
    """A manager for different sinks to store position data of the particles.
    Currently supported are:
    - Text: space delimited columns of either cartesian coordinates (x, y, z) or spherical coordinates (r, theta phi)
      or (phi, theta, r)
    """
    def __init__(self, filename, filetype="Text", verbose=True, **kwargs):
        """
        @param filename The filename of the Sink file which will be created/overwritten
        @param filetype The type of sink. Currently supported is only 'Text'
        @param verbose verbosity

        For Text sources, you can further specify:
        - coord: either 'cartesian', 'spherical' (r, theta, phi) or 'spherical_3dex' (phi, theta, r)
        - hubble_param: If coordinates are to be stored in Mpc/h, use this parameter for conversion.
        - box_origin: If cartesian coordinates, specify the "center of the box"
        """
        if filetype is None:
            extension = filename.split('.')[-1]
            if extension in ['txt','dat']:
                filetype = 'Text'
            else:
                raise RuntimeError("Could not derrive the filetype, please specify explicitly.")

        assert(filetype in ["Text"])

        hubble_param = kwargs.get('hubble_param', 1)
        box_origin = kwargs.get('box_origin', 0)
        coord = kwargs.get('coord', 'cartesian')
        assert (coord in ['cartesian', 'spherical', 'spherical_3dex'])

        if filetype == "Text":
            if coord == 'cartesian':
                self.sink = io_core.TextSink_cartesian(filename, verbose)
            elif coord == 'spherical':
                self.sink = io_core.TextSink_spherical(filename, verbose)
            elif coord == 'spherical_3dex':
                self.sink = io_core.TextSink_spherical_3dex(filename, verbose)


        else:
            raise RuntimeError("Unsupported filetype: {}".format(filetype))


class PyFilter(object):
    """A manager for different filters to filter particles depending on their position.
    Currently supported are:
    - tophat: A tophat filter which removes all particles with r>R0
    - gauss : A gaussian filter where exp(-(r/R0)**2) is the probability of accepting the particle
    - generic: Specify the acceptance probability function
    - AngMask: A HEALPix angular mask filter (give path to HEALPix map where 1->accept and 0->reject)
    """
    def __init__(self, filter_name, filter_option, **kwargs):
        """
        @param filter_name Type of filter. See below.
        @param filter_option Options for the specified filter. See below

        For 'tophat':  filter_option=R0
        For 'gauss' :  filter_option=R0
        For 'generic': filter_option=window_function (a function with signature double(double)). Further specify rmax=rmax,
                       the maximum distance for which the function will be called
        For 'AngMask': HEALPix map or filepath to HEALPix mask. If map, you can specify the location of the temporary file
                       by also setting temp_file=path_to_temporary_file which will be created and deleted after usage.
        """
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
            if isinstance(filter_option, basestring):
                assert(os.path.isfile(filter_option))
                self.filter = io_core.AngularMaskFilter(filter_option)
            elif hasattr(filter_option, "__len__"):
                maskpath = kwargs.get("temp_file", 'tmp_mask.fits')
                hp.write_map(maskpath, filter_option)
                self.filter = io_core.AngularMaskFilter(maskpath)
                os.remove(maskpath)
            else:
                raise ValueError("filter_option for AngMask filter either mask-map or path to mask-file")

    def __call__(self, object_container):
        """ Apply filter to ObjectContainer
        @param object_container The ObjectContainer instance on which to apply filter (will be resized)
        """
        assert(isinstance(object_container, basictypes.ObjectContainer))
        self.filter(object_container)



class PyFilterStream(object):
    """ Combines PySource, PySink and PyFilter to a filtered stream which processes all objects in the source to the sink.
    If not all particles after filtering are needed, specify subsample_size which will only take a random subset of the
    remaining particles.

    Note that the sink can be an instance of PySource/PySink or ObjectContainer/PixelizedObjectContainer, so that the
    data is load into the Containers directly.
    """

    def __init__(self, source, sink, subsample_size=None, temp_file=None, verbose=True):
        """
        @param source Either a PySource, an io_core.Source or an ObjectContainer instance or a list of such instances
        @param sink   Either a PySink, an io_core.Sink or an ObjectContainer/PixelizedObjectContainer instance
        @param subsample_size Only write a random subset of the filtered data to sink
        @param temp_file If subsample_size is turned on, the temporary file which is used to store the intermediary data
        can be specified here
        @param verbose verbosity
        """

        # Source(s)
        if not hasattr(source, "__len__"):
            source = [source]

        self.source = []
        for s in source:
            if isinstance(s, PySource):
                self.source.append(s.source)
            elif isinstance(s, io_core.Source):
                self.source.append(s)
            elif isinstance(s, basictypes.ObjectContainer):
                self.source.append(io_core.ObjectContainerSource(s))
            else:
                raise ValueError("unknown source: {}\n Must be either instance of PySource, Source or ObjectContainer".format(
                    type(s)
                ))

        # Sink
        if isinstance(sink, PySink):
            self.sink = sink.sink
        elif isinstance(sink, io_core.Sink):
            self.sink = sink
        elif isinstance(sink, basictypes.ObjectContainer):
            self.sink = io_core.ObjectContainerSink(sink)
        elif isinstance(sink, basictypes.PixelizedObjectContainer):
            self.sink = io_core.PixelizedObjectContainerSink(sink)
        else:
            raise ValueError("sink must be either instance of PySink, Sink, ObjectContainer or PixelizedObjectContainer")

        # Subsampling
        if subsample_size is not None:
            self.subsample_size = int(subsample_size)
        else:
            self.subsample_size = 0

        self.temp_file = "tmp_{}.bin".format(os.getpid()) if temp_file is None else temp_file

        self.filter_stream = io_core.FilterStream(self.source[0], self.sink, buffer_size, verbose)
        self.filters = []
        self.verbose=verbose

    def add_filter(self, py_filter):
        """ Add a PyFilter to the FilterStream
        @param py_filter an instance of PyFilter
        """
        assert(isinstance(py_filter, PyFilter))
        self.filters.append(py_filter.filter)  # Need to store the filter objects, otherwise destroyed
        self.filter_stream.add_filter(py_filter.filter)

    def run(self):
        """ Run the filter stream: process all objects from source (filtering) and write to sink
        """

        if len(self.source) == 1:
            self.filter_stream.run(self.subsample_size, self.temp_file)
        else:
            if self.verbose:
                print("Processing file {} of {}".format(1, len(self.source)+1))
            self.filter_stream.run_totemp(self.temp_file, append=False)
            for i,s in enumerate(self.source[1:]):
                if self.verbose:
                    print("Processing file {} of {}".format(i+2, len(self.source)+1))
                self.filter_stream.set_source(s)
                self.filter_stream.run_totemp(self.temp_file, append=True)

            self.filter_stream.run_fromtemp(self.temp_file, self.subsample_size, remove_temp=True)