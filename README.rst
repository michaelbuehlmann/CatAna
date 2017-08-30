.. code-block:: none

     ___    __   ____   __    _  _    __                                                /\
    / __)  /__\ (_  _) /__\  ( \( )  /__\       ,--------------------------------------/ vvvvvvvvvvv\
   ( (__  /(__)\  )(  /(__)\  )  (  /(__)\       "=====================================\^^^^^^^^^^^^'
    \___)(__)(__)(__)(__)(__)(_)\_)(__)(__)                                             \/
                                                                                     Michael Bühlmann
   Catalog Analyzer: A library for Spherical Fourier Bessel (SFB) Decomposition
   ==================================================================================================


Requirements
============

- cmake >= 3.2.0
- GNU Scientific Library (GSL) and headers
- FFTW3 library and headers
- CFITSIO library and headers
- Compiler supporting C++14 (gcc >= 5.0, clang >= 3.4)

The library is only being tested on Python 3.5. A compiler supporting OpenMP is recommended to take advantage of the
speedup from parallelization.

For the following steps make sure that your ``CC`` and ``CXX`` variables are set to the correct compiler and that
``python`` is the correct python executable. Using a virtual environment is recommended.

Installation
============

.. code-block: bash

   pip install catana

Documentation
=============

Documentation can be found under `https://michaelbuehlmann.github.io/CatAna/ <https://michaelbuehlmann.github.io/CatAna/>`_

The code is hosted on `GitHub <https://github.com/michaelbuehlmann/CatAna>`_