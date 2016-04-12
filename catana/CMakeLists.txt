# SOURCE FILES
set(tools_SOURCES
        include/catana/tools/gsl_function_wrapper.hpp
        include/catana/tools/FunctionInterpolator.hpp src/tools/FunctionInterpolator.cpp
        include/catana/tools/string_format.hpp)

set(besseltools_SOURCES
        include/catana/besseltools.hpp
        ${tools_SOURCES}
        include/catana/besseltools/SphericalBesselZeros.hpp src/besseltools/SphericalBesselZeros.cpp
        include/catana/besseltools/sbessel_integrator.hpp src/besseltools/sbessel_integrator.cpp
        include/catana/besseltools/SBesselInterpolator.hpp src/besseltools/SBesselInterpolator.cpp)

set(basictypes_SOURCES
        include/catana/objects/Object.hpp src/objects/Object.cpp
        include/catana/objects/ObjectContainer.hpp src/objects/ObjectContainer.cpp
        include/catana/objects/PixelizedObjectContainer.hpp src/objects/PixelizedObjectContainer.cpp)

set(returntypes_SOURCES
        include/catana/decomposition/return_types.hpp src/decomposition/return_types.cpp)

set(io_sources_SOURCES
        include/catana/io/Source.hpp src/io/Source.cpp
        include/catana/io/sources/ObjectContainerSource.hpp src/io/sources/ObjectContainerSource.cpp
        include/catana/io/sources/GadgetSource.hpp src/io/sources/GadgetSource.cpp
        include/catana/io/sources/RawBinarySource.hpp src/io/sources/RawBinarySource.cpp
        include/catana/io/sources/TextSource.hpp src/io/sources/TextSource.cpp)

set(io_sinks_SOURCES
        include/catana/io/Sink.hpp
        include/catana/io/sinks/ObjectContainerSink.hpp
        include/catana/io/sinks/PixelizedObjectContainerSink.hpp
        include/catana/io/sinks/RawBinarySink.hpp src/io/sinks/RawBinarySink.cpp
        include/catana/io/sinks/TextSink.hpp src/io/sinks/TextSink.cpp
        )

set(io_filters_SOURCES
        include/catana/io/Filter.hpp src/io/Filter.cpp
        include/catana/io/filters/radial_filters.hpp src/io/filters/radial_filters.cpp
        include/catana/io/filters/angular_filters.hpp src/io/filters/angular_filters.cpp
        )

set(io_SOURCES
        ${basictypes_SOURCES}
        ${tools_SOURCES}
        include/catana/io/record_types.hpp src/io/record_type.cpp
        ${io_sources_SOURCES}
        ${io_sinks_SOURCES}
        ${io_filters_SOURCES}
        include/catana/io/FilterStream.hpp src/io/FilterStream.cpp)

set(decomposition_SOURCES
        ${returntypes_SOURCES}
        include/catana/decomposition/sfb_decomposition.hpp src/decomposition/sfb_decomposition.cpp
        include/catana/decomposition/sfb_raw.hpp src/decomposition/sfb_raw.cpp
        include/catana/decomposition/sfb_reverse.hpp src/decomposition/sfb_reverse.cpp
        include/catana/decomposition/sfb_reverse_fft.hpp src/decomposition/sfb_reverse_fft.cpp
        src/decomposition/Analyzer.cpp include/catana/decomposition/Analyzer.hpp)

# COMPILE AS STATIC LIBRARY
#add_library(io STATIC ${io_SOURCES})
#add_library(besseltools STATIC ${besseltools_SOURCES})
add_library(catana STATIC
        ${io_SOURCES}
        ${besseltools_SOURCES}
        ${decomposition_SOURCES})

# LINK TOGETHER
#target_link_libraries(besseltools ${GSL_LIBRARIES} ${CBLAS_LIBRARIES})
#target_link_libraries(io ${GSL_LIBRARIES} ${CBLAS_LIBRARIES} ${HEALPIX_LIBRARIES} ${CFITSIO_LIBRARIES})
#target_link_libraries(catana besseltools io ${GSL_LIBRARIES} ${CBLAS_LIBRARIES} ${FFTW3_LIBRARIES} ${HEALPIX_LIBRARIES} ${CFITSIO_LIBRARIES})
target_link_libraries(catana ${GSL_LIBRARIES} ${CBLAS_LIBRARIES} ${FFTW3_LIBRARIES} ${HEALPIX_LIBRARIES} ${CFITSIO_LIBRARIES})
add_dependencies(catana eigen)
#add_dependencies(io eigen)
