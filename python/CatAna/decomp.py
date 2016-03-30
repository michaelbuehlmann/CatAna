"""
This module is a convenience wrapper of the CatAna.decomp_core python module.

The CatAna.decomp_core module directly exposes the CatAna "decomp" (decomposition) library functions written in C++.
This module is a convenience wrapper around the CatAna.decomp_core module for simplified usage.
"""

from __future__ import division, print_function, absolute_import

import numpy as np

from . import basictypes
from . import decomp_core
from . import io_core
from . import io

decomp_core.init_random()

sfb_decomposition = decomp_core.sfb_decomposition
KClkk = decomp_core.KClkk

class PyAnalyzer(object):
    def __init__(self, py_source, window_volume, subsample_size=None):
        if isinstance(py_source, io.PySource):
            self.source = py_source.source
        elif isinstance(py_source, io_core.Source):
            self.source = py_source
        elif isinstance(py_source, basictypes.ObjectContainer):
            self.source = io_core.ObjectContainerSource(py_source)
        else:
            raise ValueError("py_source must be instance of PySource, Source or ObjectContainer")
        self.window_volume = window_volume

        self.analyzer = decomp_core.Analyzer(self.source, self.window_volume)
        if subsample_size is not None:
            self.analyzer.set_subsample_size(int(subsample_size))
        self.filters = []

    def add_filter(self, py_filter):
        assert(isinstance(py_filter, io.PyFilter))
        self.filters.append(py_filter.filter)
        self.analyzer.add_filter(py_filter.filter)

    def compute_sfb(self, lmax, nmax, rmax, store_flmn=False, verbose=True):
        return self.analyzer.compute_sfb(lmax, nmax, rmax, store_flmn, verbose)

    def compute_sfb_pixelized(self, lmax, nmax, rmax, nside, store_flmn=False, verbose=True):
        return self.analyzer.compute_sfb_pixelized(lmax, nmax, rmax, nside, store_flmn, verbose)