add_executable(runBesselZerosCheck
        zeros.cpp)

target_link_libraries(runBesselZerosCheck gtest gtest_main)
target_link_libraries(runBesselZerosCheck besseltools)
