cmake_minimum_required(VERSION 3.3)
project(catana)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -fPIC")
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


# HDF5 Library
set(HDF5_USE_STATIC_LIBRARIES)
find_package(HDF5 REQUIRED)
include_directories(${HDF5_INCLUDE_DIRS})
link_directories(${HDF5_LIBRARY_DIRS})

include_directories(catana/include)

add_subdirectory(catana)
add_subdirectory(python)

if(TEST)
    add_subdirectory(test)
endif()
