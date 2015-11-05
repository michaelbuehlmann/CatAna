cmake_minimum_required(VERSION 3.3)
project(catana_app)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11")

# set PYTHON to use virtualenvironment. If commented out,
# the standard python interpreter will be used.
set(PYTHON /Users/michael/.virtualenvs/MT/bin/python)

# git versioning (python package will be of this version)
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