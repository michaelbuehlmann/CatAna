# #######################
# GOOGLE TEST LIBRARY ###
# #######################
project(catana_tests)

# Moving test data for IO Tests
file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(${GOOGLETEST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Google Tests subdirectories
#add_subdirectory(besseltools_tests)
#add_subdirectory(tools_tests)
#add_subdirectory(io_tests)
#add_subdirectory(object_types_tests)
#add_subdirectory(decomposition_tests)

# Timing subdirectories
add_subdirectory(speed_comparisons)


#####################
# ALL TESTS IN ONE: #
file(GLOB TEST_SOURCES
        /besseltools_tests/*.cpp
        object_types_tests/*.cpp
        tools_tests/*.cpp
        field_test/*.cpp
        io_tests/*.cpp
        decomposition_tests/*.cpp)
add_executable(GTESTS main.cpp ${TEST_SOURCES})
target_compile_definitions(GTESTS PRIVATE "-DALL_TESTS" )
target_link_libraries(GTESTS
        gtest gtest_main
#        io catana besseltools
        catana
        ${CBLAS_LIBRARIES} ${HEALPIX_LIB})
