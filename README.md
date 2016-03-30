```
  ___    __   ____   __    _  _    __                                                /\
 / __)  /__\ (_  _) /__\  ( \( )  /__\       ,--------------------------------------/ vvvvvvvvvvv\
( (__  /(__)\  )(  /(__)\  )  (  /(__)\       "=====================================\^^^^^^^^^^^^'
 \___)(__)(__)(__)(__)(__)(_)\_)(__)(__)                                             \/
                                                                                  Michael Bühlmann
Catalog Analyzer: A library for Spherical Fourier Bessel (SFB) Decomposition
==================================================================================================
```

# Requirements {#README}
- C++11 compatible compiler (GCC for OpenMP)
- FFTW3 library
- GSL library
- HEALPix library (C++)  
  (compile with either 'generic_gcc' or 'optimized_gcc' config, since 'basic_gcc' does not 
  produce position independent code (needs compilation option -fPIC))

# Optional requirements
- python and python developement library for python libraries  
  (also: numpy and healpy Python modules)
- doxygen and doxypypy (can be installed using pip) for documentation

# Installation
- `git clone` the repository and `cd catana` into the main folder
- run `git submodule init` and `git submodule update` to initialize the external modules needed (Eigen, Pybind11, GoogleTest)
- run `cp config.cmake.example config.cmake` and edit the config.cmake file for your system.
- run `mkdir build` to create build folder, and within build, run `cmake .. -DCMAKE_BUILD_TYPE=Release`    
  to choose compiler, run `export CC={path_to_ccompiler}` and `export CXX={path_to_cppcompiler}` before cmake, e.g.
  `export CC=/usr/local/bin/gcc-5` and `export CXX=/usr/local/bin/g++-5`
- run `make catana` to create the C++ CatAna library (within `./catana`)
- run `make python` to create the Python Libraries (within `./python/`)
- run `make python_install` to install the Python Libraries automatically or run `python setup.py install` 
  within the `.python` subfolder manually

# Further Documentation
- run `make doc` to create the doxygen documentation (within `./docs`)
- open `./docs/html/index.html` in your browser

# Tests
- run `make GTESTS` to create all the tests
- run `./test/GTESTS` to run the tests
