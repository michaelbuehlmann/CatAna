include_directories("../external/doctest/doctest")
file(COPY test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY speed_comparisons/sfb_decomp_time_paramscale.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(TEST_FILES
        main.cpp
        tools_tests/interpolation_tests.cpp
        tools_tests/return_types_test.cpp
        point_types_tests/point_types_tests.cpp
        besseltools_tests/SphericalBesselZeros_test.cpp
        besseltools_tests/SBesselLookUp_test.cpp
        besseltools_tests/sbessel_integrator.cpp
        decomposition_tests/analyzer_test.cpp
        decomposition_tests/sfb_test.cpp
        io_tests/filter_stream_test.cpp
        io_tests/filter_tests.cpp
        io_tests/pixelized_oc_sink_test.cpp
        io_tests/read_gadget_tests.cpp
        io_tests/readwrite_rawdata_tests.cpp
        io_tests/readwrite_text_tests.cpp
        )

add_executable(TESTS ${TEST_FILES})
target_link_libraries(TESTS catana)


set(TIMER_FILES
        speed_comparisons/timer.hpp speed_comparisons/timer.cpp)

add_executable(SPEED_threadscale
        speed_comparisons/sfb_decomp_threadscale.cpp
        ${TIMER_FILES})
target_link_libraries(SPEED_threadscale catana)

add_executable(SPEED_paramscale
        speed_comparisons/sfb_decomp_time_paramscale.cpp
        ${TIMER_FILES}
        speed_comparisons/json.hpp)
target_link_libraries(SPEED_paramscale catana)