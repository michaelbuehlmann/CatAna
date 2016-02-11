# #######################
# GOOGLE TEST LIBRARY
# #######################
project(catana_tests)
file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#add_definitions(-DTEST_DATA_DIR="\\"${CMAKE_CURRENT_BINARY_DIR}/test_data/\\"")
configure_file(config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/config.hpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)


add_subdirectory(${GOOGLETEST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

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
target_compile_definitions(GTESTS PRIVATE "-DALL_TESTS" )
target_link_libraries(GTESTS
        gtest gtest_main
        iotools catana besseltools
        ${CBLAS_LIBRARIES} ${HEALPIX_LIB})
