project(catana)
include_directories(include)

set(besseltools_SOURCES src/tools/besseltools.cpp include/catana/tools/besseltools.hpp)
set(catalog_SOURCES src/catalog.cpp include/catana/catalog.hpp)
set(field_SOURCES src/field.cpp include/catana/field.hpp)

add_library(besseltools STATIC ${besseltools_SOURCES})
target_link_libraries(besseltools gsl)

add_library(catalog STATIC ${catalog_SOURCES})
add_library(field STATIC ${field_SOURCES})

#set(BESSEL_TEST_SOURCES
#        ../test/tools_tester.cpp
#        src/tools/besseltools.cpp
#        include/catana/tools/besseltools.hpp
#        include/catana/tools/timer.hpp
#        src/tools/timer.cpp
#        )
#add_executable(BesselTester ${BESSEL_TEST_SOURCES})
#target_link_libraries(BesselTester gsl)


