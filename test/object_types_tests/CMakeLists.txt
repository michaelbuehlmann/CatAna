add_executable(object_types_tests object_types_tests.cpp)

target_link_libraries(object_types_tests gtest gtest_main)
target_link_libraries(object_types_tests ${HEALPIX_LIB})
