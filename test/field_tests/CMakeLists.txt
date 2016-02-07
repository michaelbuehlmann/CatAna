include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(field_basics field_basics.cpp)

target_link_libraries(field_basics gtest gtest_main)
target_link_libraries(field_basics catana)
