add_executable(sfb_test sfb_test.cpp)
target_link_libraries(sfb_test gtest gtest_main)
target_link_libraries(sfb_test catana)

add_executable(analyzer_test analyzer_test.cpp)
target_link_libraries(analyzer_test gtest gtest_main)
target_link_libraries(analyzer_test catana)