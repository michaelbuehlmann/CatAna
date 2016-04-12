cmake_minimum_required(VERSION 2.8)
project(catana)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

# Load modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
message(STATUS "module path: ${CMAKE_MODULE_PATH}")

# Load config.cmake
if(EXISTS "${PROJECT_SOURCE_DIR}/config.cmake")
    message(STATUS "Loaded external config file")
    include(${PROJECT_SOURCE_DIR}/config.cmake)
endif()

# Load git versioning module
include(${CMAKE_SOURCE_DIR}/cmake/git_versioning.cmake)

# Load external projects (eigen, pybind11, googletest)
include(ExternalProject)
include(${CMAKE_SOURCE_DIR}/cmake/eigen3_external.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/pybind11_external.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/googletest_external.cmake)

################################################################################
# LOADING LIBRARIES ############################################################
################################################################################
# FFTW3 Library
find_package(FFTW3 REQUIRED)
include_directories(${FFTW3_INCLUDES})

# CBLAS Library
find_package(CBLAS REQUIRED)
include_directories(${CBLAS_INCLUDE_DIR})
message(STATUS "CBLAS Libraries: ${CBLAS_LIBRARIES}")

# GSL Library
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDES})

# eigen3 Library
message(STATUS "Using Eigen Library located at ${EIGEN3_INCLUDE_DIR}")
include_directories(${EIGEN3_INCLUDE_DIR})

# pybind11 Library
message(STATUS "Using Pybind11 Library located at ${PYBIND11_INCLUDE_DIR}")
include_directories(${PYBIND11_INCLUDE_DIR})

# HEALPix Library
find_package(CFITSIO REQUIRED)
if(NOT (HEALPIX_INCLUDEPATH AND HEALPIX_LIBPATH))
    message(STATUS "No existing HEALPIX installation was given. Will attempt to automatically download and install...")
    message(STATUS "(otherwise, specify HEALPIX_INCLUDEPATH and HEALPIX_LIBPATH within config.cmake)")
    include(${CMAKE_SOURCE_DIR}/cmake/healpix_external.cmake)
else()
    find_package(HEALPIX REQUIRED)
    include_directories(${HEALPIX_INCLUDE_DIR})
endif()


# OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
message(STATUS "CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
# Python
if(BUILD_PYTHON)
    find_package(PythonLibs REQUIRED)
    find_package(PythonInterp REQUIRED)
endif()


# Setting up include files from CatAna
#if(CATANA_CONFIG_DIR)
configure_file(catana/include/catana/config.hpp.in catana/include/catana/config.hpp)
include_directories(catana/include)
include_directories(${CMAKE_BINARY_DIR}/catana/include)

add_subdirectory(catana)
add_subdirectory(docs)

if(BUILD_PYTHON)
    add_subdirectory(python)
endif()

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
