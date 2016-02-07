include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(read_gadget_tests read_gadget_tests.cpp)
target_link_libraries(read_gadget_tests gtest gtest_main)
target_link_libraries(read_gadget_tests iotools)

add_executable(read_hdf5_tests read_hdf5_tests.cpp)
target_link_libraries(read_hdf5_tests gtest gtest_main)
target_link_libraries(read_hdf5_tests iotools)