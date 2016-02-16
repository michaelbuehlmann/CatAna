add_executable(sfb_test sfb_test.cpp)
target_link_libraries(sfb_test gtest gtest_main)
target_link_libraries(sfb_test catana)