# SOURCE FILES
set(CATANA_TOOLS_SOURCES
        include/catana/tools/gsl_function_wrapper.hpp
        include/catana/tools/FunctionInterpolator.hpp src/tools/FunctionInterpolator.cpp
        include/catana/tools/string_format.hpp
        src/tools/random.cpp include/catana/tools/random.hpp)

set(CATANA_BESSELTOOLS_SOURCES
        include/catana/besseltools/SphericalBesselZeros.hpp src/besseltools/SphericalBesselZeros.cpp
        include/catana/besseltools/sbessel_integrator.hpp src/besseltools/sbessel_integrator.cpp
        include/catana/besseltools/SBesselInterpolator.hpp src/besseltools/SBesselInterpolator.cpp)

set(CATANA_BASICTYPES_SOURCES
        include/catana/points/Point.hpp src/points/Point.cpp
        include/catana/points/PointContainer.hpp src/points/PointContainer.cpp
        include/catana/points/PixelizedPointContainer.hpp src/points/PixelizedPointContainer.cpp
        include/catana/decomposition/return_types.hpp src/decomposition/return_types.cpp)

set(CATANA_IO_SOURCES
        include/catana/io/Source.hpp src/io/Source.cpp
        include/catana/io/sources/PointContainerSource.hpp src/io/sources/PointContainerSource.cpp
        include/catana/io/sources/GadgetSource.hpp src/io/sources/GadgetSource.cpp
        include/catana/io/sources/RawBinarySource.hpp src/io/sources/RawBinarySource.cpp
        include/catana/io/sources/TextSource.hpp src/io/sources/TextSource.cpp
        include/catana/io/Sink.hpp src/io/Sink.cpp
        include/catana/io/sinks/PointContainerSink.hpp
        include/catana/io/sinks/PixelizedPointContainerSink.hpp
        include/catana/io/sinks/RawBinarySink.hpp src/io/sinks/RawBinarySink.cpp
        include/catana/io/sinks/TextSink.hpp src/io/sinks/TextSink.cpp
        include/catana/io/Filter.hpp src/io/Filter.cpp
        include/catana/io/filters/radial_filters.hpp src/io/filters/radial_filters.cpp
        include/catana/io/filters/angular_filters.hpp src/io/filters/angular_filters.cpp
        include/catana/io/record_types.hpp src/io/record_type.cpp
        include/catana/io/FilterStream.hpp src/io/FilterStream.cpp)

set(CATANA_DECOMPOSITION_SOURCES
        include/catana/decomposition/sfb_decomposition.hpp src/decomposition/sfb_decomposition.cpp
        include/catana/decomposition/sfb_raw.hpp src/decomposition/sfb_raw.cpp
        include/catana/decomposition/sfb_reverse.hpp src/decomposition/sfb_reverse.cpp
        include/catana/decomposition/sfb_reverse_fft.hpp src/decomposition/sfb_reverse_fft.cpp
        include/catana/decomposition/Analyzer.hpp src/decomposition/Analyzer.cpp)

# COMPILE AS STATIC LIBRARY
add_library(catana
        include/catana/catana.hpp
        ${CATANA_TOOLS_SOURCES}
        ${CATANA_BESSELTOOLS_SOURCES}
        ${CATANA_BASICTYPES_SOURCES}
        ${CATANA_IO_SOURCES}
        ${CATANA_DECOMPOSITION_SOURCES})


# LINK TOGETHER
target_link_libraries(catana
        ${GSL_LIBRARIES}
        ${CBLAS_LIBRARIES}
        ${FFTW3_LIBRARIES}
        ${HEALPIX_LIBRARIES}
        ${CFITSIO_LIBRARIES})

# LINK TO ZLIB
find_package(ZLIB)
if (${ZLIB_FOUND})
    target_link_libraries(catana ${ZLIB_LIBRARIES})
endif ()

add_dependencies(catana eigen)
