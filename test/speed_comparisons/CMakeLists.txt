file(COPY sfb_decomp_time_monch.sh DESTINATION .)

add_executable(sfb_time sfb_decomp_time.cpp timer.hpp)
target_link_libraries(sfb_time catana)

if(OPENMP_FOUND)
    add_executable(sfb_decomp_threadscale sfb_decomp_threadscale.cpp timer.hpp)
    target_link_libraries(sfb_decomp_threadscale catana)
endif()