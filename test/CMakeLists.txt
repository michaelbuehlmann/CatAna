# #######################
# GOOGLE TEST LIBRARY
# #######################
add_subdirectory(${GOOGLETEST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

project(catana_tests)

add_subdirectory(besseltools_tests)
add_subdirectory(field_tests)
add_subdirectory(io_tests)
add_subdirectory(object_types_tests)

# ALL TESTS:
file(GLOB TEST_SOURCES
        /besseltools_tests/*.cpp
        field_test/*.cpp
        io_tests/*.cpp
        object_types_tests/*.cpp)
add_executable(GTESTS main.cpp ${TEST_SOURCES})
target_link_libraries(GTESTS gtest gtest_main iotools catana besseltools)