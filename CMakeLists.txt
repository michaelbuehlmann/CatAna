cmake_minimum_required(VERSION 3.3)
project(catana_app)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11")

# set PYTHON to use virtualenvironment. If commented out,
# the standard python interpreter will be used.
set(PYTHON /Users/michael/.virtualenvs/MT/bin/python)

include_directories(catana/include)

add_subdirectory(catana)
add_subdirectory(test)
add_subdirectory(python)