cmake_minimum_required(VERSION 3.3)
project(catana)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")

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

# eigen3 Library
message(STATUS "Using Eigen Library located at ${EIGEN3_INCLUDE_DIR}")
include_directories(${EIGEN3_INCLUDE_DIR})

# pybind11 Library
message(STATUS "Using Pybind11 Library located at ${PYBIND11_INCLUDE_DIR}")
include_directories(${PYBIND11_INCLUDE_DIR})

# HEALPix Library
find_package(CFITSIO REQUIRED)
find_package(HEALPIX REQUIRED)
include_directories(${HEALPIX_INCLUDE_DIR})
link_directories(${HEALPIX_LIB_DIR})

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
