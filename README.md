```
  ___    __   ____   __    _  _    __                                                /\
 / __)  /__\ (_  _) /__\  ( \( )  /__\       ,--------------------------------------/ vvvvvvvvvvv\
( (__  /(__)\  )(  /(__)\  )  (  /(__)\       "=====================================\^^^^^^^^^^^^'
 \___)(__)(__)(__)(__)(__)(_)\_)(__)(__)                                             \/
                                                                                  Michael Bühlmann
Catalog Analyzer: A library for Spherical Fourier Bessel (SFB) Decomposition
==================================================================================================
```

# Requirements
- C++11 compatible compiler (GCC for OpenMP)
- FFTW3 library
- HEALPix library (C++)
- GSL library

# Optional requirements
- python and python developement library for python libraries
- doxygen for documentation

# Installation
- `git clone` the repository and `cd catana` into the main folder
- run `git submodule init` and `git submodule update` to initialize the external modules needed (Eigen, Pybind11, GoogleTest)
- run `cp config.cmake.example config.cmake` and edit the config.cmake file for your system.
- run `mkdir build` to create build folder, and within build, run `cmake .. -DCMAKE_BUILD_TYPE=Release`    
  to choose compiler, run `export CC={path_to_ccompiler}` and `export CXX={path_to_cppcompiler}` before cmake, e.g.
  `export CC=/usr/local/bin/gcc-5` and `export CXX=/usr/local/bin/g++-5`
- run `make python_libraries` to create the Python Libraries (within `./python/CatAna`)
- run `make catana` to create the C++ CatAna library
- run `make GTESTS` to create all the tests (run `./test/GTESTS`)
- run `make doc` to create the doxygen documentation (within `./docs`)