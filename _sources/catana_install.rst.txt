Installation
============

Requirements
------------

- cmake >= 3.2.0
- GNU Scientific Library (GSL) and headers
- FFTW3 library and headers
- CFITSIO library and headers
- Compiler supporting C++14 (gcc >= 5.0, clang >= 3.4)

The library is only being tested on Python 3.5. A compiler supporting OpenMP is recommended to take advantage of the
speedup from parallelization.

For the following steps make sure that your ``CC`` and ``CXX`` variables are set to the correct compiler and that
``python`` is the correct python executable. Using a virtual environment is recommended.


Installation from git source
----------------------------

.. code-block:: bash

   git clone --recursive https://github.com/michaelbuehlmann/CatAna.git
   cd CatAna
   python setup.py install

To check if everything works fine, you can compile and run the (C++) tests:

.. code-block:: bash

   mkdir build && cd build
   cmake -DBUILD_TESTS=ON ..
   make
   ./test/TESTS


Installation using pip
----------------------

.. code-block:: bash

   pip install catana
