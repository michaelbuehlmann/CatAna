cmake_minimum_required(VERSION 3.3)
project(catana)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
message(STATUS "module path: ${CMAKE_MODULE_PATH}")

if(EXISTS "${PROJECT_SOURCE_DIR}/config.cmake")
    message(STATUS "Loaded external config file")
    include(${PROJECT_SOURCE_DIR}/config.cmake)
endif()

# Load/download external modules
include(${CMAKE_SOURCE_DIR}/external_projects.cmake)

# eigen3 Library
message(STATUS "Using Eigen Library located at ${EIGEN3_INCLUDE_DIR}")
include_directories(${EIGEN3_INCLUDE_DIR})
# pybind11 Library
message(STATUS "Using Pybind11 Library located at ${PYBIND11_INCLUDE_DIR}")
include_directories(${PYBIND11_INCLUDE_DIR})
# HEALPix Library
include_directories(${HEALPIX_DIR}/include)
link_directories(${HEALPIX_DIR}/lib)
if(EXISTS ${HEALPIX_DIR}/lib/libcxxsupport.a)
    set(HEALPIX_LIB healpix_cxx cxxsupport cfitsio)
    message(STATUS "HEALPix Library consists of multiple files: ${HEALPIX_LIB}")
else()
    set(HEALPIX_LIB healpix_cxx)
    message(STATUS "HEALPix Library is single file: ${HEALPIX_LIB}")
endif()

# CBLAS Library
set(CBLAS_FIND_QUIETLY)
find_package(CBLAS REQUIRED)
include_directories(${CBLAS_INCLUDE_DIR})
message(STATUS "CBLAS Libraries: ${CBLAS_LIBRARIES}")

# OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Setting up include files from CatAna
#if(CATANA_CONFIG_DIR)
configure_file(catana/include/catana/config.hpp.in catana/include/catana/config.hpp)
include_directories(catana/include)
include_directories(${CMAKE_BINARY_DIR}/catana/include)

add_subdirectory(catana)

if(BUILD_PYTHON)
    add_subdirectory(python)
endif()

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
