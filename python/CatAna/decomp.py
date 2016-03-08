from __future__ import division, print_function, absolute_import

import numpy as np

from . import decomp_core
from . import io_core
from . import io

decomp_core.init_random()

sfb_decomposition = decomp_core.sfb_decomposition
KClkk = decomp_core.KClkk

class PyAnalyzer(object):
    def __init__(self, py_source, window_volume, subsample_size=None):
        assert(isinstance(py_source, io.PySource))
        self.source = py_source.source
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