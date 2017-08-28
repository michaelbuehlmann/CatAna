Installation
============

Requirements
------------

- cmake >= 3.2.0
- GNU Scientific Library (GSL) and headers
- FFTW3 library and headers
- CFITSIO library and headers
- Compiler supporting C++14 (gcc >= 5.0, clang >= 3.4)

The library is only tested on Python 3.5. A compiler supporting OpenMP is recommended to profit from parallelization
speedup.


Installation from git source
----------------------------

.. code-block:: bash

   git clone --recursive https://github.com/michaelbuehlmann/CatAna.git
   cd CatAna
   python setup.py install



Installation using pip
----------------------

.. code-block:: bash

   pip install catana

