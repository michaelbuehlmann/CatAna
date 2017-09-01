cmake_minimum_required(VERSION 3.2)
project(CatAna)

# Load config.cmake
if (EXISTS "${PROJECT_SOURCE_DIR}/config.cmake")
    message(STATUS "Loaded external config file")
    include(${PROJECT_SOURCE_DIR}/config.cmake)
else ()
    message(STATUS "No config.cmake provided.")
    option(BUILD_TESTS "Build the test programs" OFF)
    option(BUILD_PYTHON "Build the CatAna Python Library" ON)
endif ()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wall -O3")

# Set the correct compiler flags if we build the python modules
if(${BUILD_PYTHON})
    add_subdirectory(external/pybind11)
endif()

# Load modules
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake/modules")
message(STATUS "module path: ${CMAKE_MODULE_PATH}")
include(ExternalProject)

# Get CatAna version
include(GetGitRevisionDescription)
git_describe(VERSION --tags --dirty --always)
message(STATUS "CatAna Version: ${VERSION}")


################################################################################
# LOADING LIBRARIES ############################################################
################################################################################
# FFTW3 Library
find_package(FFTW3 REQUIRED)
include_directories(${FFTW3_INCLUDES})

# CBLAS Library
#find_package(CBLAS REQUIRED)
#include_directories(${CBLAS_INCLUDE_DIR})
#message(STATUS "CBLAS Libraries: ${CBLAS_LIBRARIES}")

# GSL Library
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})

# eigen3 Library
include(${CMAKE_SOURCE_DIR}/cmake/eigen3_external.cmake)
message(STATUS "Using Eigen Library located at ${EIGEN3_INCLUDE_DIR}")
include_directories(${EIGEN3_INCLUDE_DIR})

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


################################################################################
# CATANA LIBRARY ###############################################################
################################################################################
configure_file(catana/include/catana/config.hpp.in catana/include/catana/config.hpp)
include_directories(catana/include)
include_directories(${CMAKE_BINARY_DIR}/catana/include)
add_subdirectory(catana)


################################################################################
# TESTS ########################################################################
################################################################################

if (${BUILD_TESTS})
    add_subdirectory(test)
endif ()


################################################################################
# CATANA PYTHON LIBRARY ########################################################
################################################################################
if(${BUILD_PYTHON})
    add_subdirectory(python)
endif()
