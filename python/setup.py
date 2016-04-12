#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Always prefer setuptools over distutils
from setuptools import setup, find_packages, Extension
import os
from os import path
import re

here = path.abspath(path.dirname(__file__))
os.environ['CC'] = "${CMAKE_C_COMPILER}"
os.environ['CXX'] = "${CMAKE_CXX_COMPILER}"


include_dirs = [
    "${PROJECT_SOURCE_DIR}/catana/include",
    "${PROJECT_BINARY_DIR}/catana/include",
    "${PYBIND11_INCLUDE_DIR}",
    "${EIGEN3_INCLUDE_DIR}",
    "${HEALPIX_INCLUDE_DIR}",
    "${PYTHON_INCLUDE_DIR}"
]

library_dirs = [
    "${LIB_LOCATION}"
]

libraries = [
    'catana',
    'io',
    'besseltools',
]

extra_compile_args = "${CMAKE_CXX_FLAGS}".split()
extra_link_args = "${CMAKE_CXX_FLAGS}".split()

# GSL
gsl_libs = re.split('[; ]',"${GSL_LIBRARIES}")
for gsl_lib in gsl_libs:
    extra_link_args.append(gsl_lib)

# HEALPIX
healpix_libs = re.split('[; ]',"${HEALPIX_LIBRARIES}")
for hp_lib in healpix_libs:
    if path.isfile(hp_lib):
        extra_link_args.append(hp_lib)
    else:
        libraries.append(hp_lib)

# CFITSIO
cfitsio_libs = re.split('[; ]',"${CFITSIO_LIBRARIES}")
for cf_lib in cfitsio_libs:
    extra_link_args.append(cf_lib)

# FFTW
fftw_libs = re.split('[; ]', "${FFTW3_LIBRARIES}")
for fftw_lib in fftw_libs:
    extra_link_args.append(fftw_lib)

# CBLAS
cblas_libs = re.split('[; ]', "${CBLAS_LIBRARIES}")
for cblas_lib in cblas_libs:
    cblas_path, cblas_name = path.split(cblas_lib)
    cblas_fname, cblas_ext = cblas_name.split('.')
    if cblas_ext == 'framework':
        extra_link_args.append("-framework")
        extra_link_args.append(cblas_fname)
    else:
        extra_link_args.append(cblas_lib)


# Add library directories and libraries to link_arg
for library_dir in library_dirs:
    extra_link_args.append("-L{}".format(library_dir))

for library in libraries:
    extra_link_args.append("-l{}".format(library))

module_basictypes = Extension('basictypes',
                              include_dirs = include_dirs,
                              sources = ['${CMAKE_CURRENT_SOURCE_DIR}/py_basictypes.cpp'],
                              extra_compile_args = extra_compile_args,
                              extra_link_args = extra_link_args,
                              language = 'c++')

module_besseltools = Extension('besseltools',
                               include_dirs = include_dirs,
                               sources = ['${CMAKE_CURRENT_SOURCE_DIR}/py_besseltools.cpp'],
                               extra_compile_args = extra_compile_args,
                               extra_link_args = extra_link_args,
                               language = 'c++')

module_io_core = Extension('io_core',
                           include_dirs = include_dirs,
                           sources = ['${CMAKE_CURRENT_SOURCE_DIR}/py_io.cpp'],
                           extra_compile_args = extra_compile_args,
                           extra_link_args = extra_link_args,
                           language = 'c++')

module_decomp_core = Extension('decomp_core',
                               include_dirs = include_dirs,
                               sources = ['${CMAKE_CURRENT_SOURCE_DIR}/py_decomposition.cpp'],
                               extra_compile_args = extra_compile_args,
                               extra_link_args = extra_link_args,
                               language = 'c++')

setup(
    name = 'CatAna',
    # version='${catana_VERSION}',
    version = '0.0',
    description = 'Catalog Analyzer: Power Spectrum and SFB decomposition of galaxy catalogs.',
    author = u'Michael Bühlmann',
    ext_package = 'CatAna',
    ext_modules = [module_basictypes, module_besseltools, module_io_core, module_decomp_core],
    packages = ['CatAna']
)