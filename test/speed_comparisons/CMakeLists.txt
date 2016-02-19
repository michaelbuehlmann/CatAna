file(COPY sfb_decomp_time_monch.sh DESTINATION .)

add_executable(sfb_time sfb_decomp_time.cpp timer.hpp)
target_link_libraries(sfb_time catana)

add_executable(sbessel_compute sbessel_compute.cpp timer.hpp)
target_link_libraries(sbessel_compute besseltools)