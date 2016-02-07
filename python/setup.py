#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Always prefer setuptools over distutils
from setuptools import setup, find_packages, Extension
# To use a consistent encoding
from os import path

here = path.abspath(path.dirname(__file__))

setup(
    name='CatAna',
    version='${catana_VERSION}',
    description='Catalog Analyzer: Power Spectrum and SFB decomposition of galaxy catalogs.',
    author=u'Michael Bühlmann',
    packages=find_packages(),
    package_data={'CatAna': ['pybesseltools.so']}
)