cmake_minimum_required(VERSION 3.3)
project(catana_app)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11")

# set PYTHON to use virtualenvironment. If commented out,
# the standard python interpreter will be used.
# TODO: find libraries/includes automatically
set(PYTHON /Users/michael/.virtualenvs/MT/bin/python)
set(PYTHON_INCLUDE_DIR /Users/michael/.virtualenvs/MT/include/python2.7/)
set(PYTHON_LIBRARY_DIR /Users/michael/.virtualenvs/MT/lib/python2.7/)
set(EIGEN3_INCLUDE_DIR /usr/local/Cellar/eigen/3.2.7/include/eigen3/)
set(HEALPIX_DIR /usr/local/Cellar/healpix/3.30)

# Loading Libraries:
include_directories(include)
# eigen3 Library
include_directories(${EIGEN3_INCLUDE_DIR})
# HEALPix Library
include_directories(${HEALPIX_DIR}/include)
link_directories(${HEALPIX_DIR}/lib)
# HDF5 Library
set(HDF5_USE_STATIC_LIBRARIES)
find_package(HDF5)
include_directories(${HDF5_INCLUDE_DIRS})
link_directories(${HDF5_LIBRARY_DIRS})

# GIT versioning
# use tags on git repository
find_package(Git)
if(GIT_FOUND)
    exec_program(
            ${GIT_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}
            ARGS "rev-parse --is-inside-work-tree"
            OUTPUT_VARIABLE GITWORKTREE )
    if( "${GITWORKTREE}" STREQUAL "true" )
        exec_program(
                ${GIT_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}
                ARGS "name-rev --tags --name-only $(git rev-parse HEAD)"
                OUTPUT_VARIABLE GITTAG )
        exec_program(
                ${GIT_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}
                ARGS "rev-parse --short HEAD"
                OUTPUT_VARIABLE GITHASH )
        exec_program(
                ${GIT_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}
                ARGS "diff-index --name-only HEAD"
                OUTPUT_VARIABLE GITDIRTY
                OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(GITDIRTY)
            set (catana_VERSION  "${GITTAG} (${GITHASH}-dirty)")
        else()
            set (catana_VERSION  "${GITTAG} (${GITHASH})")
        endif()
    endif()
endif()

include_directories(catana/include)

add_subdirectory(catana)
add_subdirectory(test)
add_subdirectory(python)
#add_subdirectory(python_old)