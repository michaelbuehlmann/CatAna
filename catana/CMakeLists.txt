project(catana)

# SOURCE FILES
set(tools_SOURCES
        include/catana/tools/gsl_function_wrapper.hpp
        include/catana/tools/fftw_field.hpp
        include/catana/tools/FunctionInterpolator.hpp src/tools/FunctionInterpolator.cpp
        include/catana/tools/string_format.hpp)

set(besseltools_SOURCES
        include/catana/besseltools.hpp
        ${tools_SOURCES}
        include/catana/besseltools/BesselZeros.hpp src/besseltools/BesselZeros.cpp
        include/catana/besseltools/sbessel_integrator.hpp src/besseltools/sbessel_integrator.cpp
        include/catana/besseltools/SBesselInterpolator.hpp src/besseltools/SBesselInterpolator.cpp)

set(basictypes_SOURCES
        include/catana/objects/Object.hpp
        include/catana/objects/ObjectContainer.hpp src/objects/ObjectContainer.cpp
        include/catana/objects/PixelizedObjectContainer.hpp src/objects/PixelizedObjectContainer.cpp)

set(returntypes_SOURCES
        include/catana/decomposition/return_types.hpp src/decomposition/return_types.cpp)

set(iotools_sources_SOURCES
        include/catana/io_tools/Source.hpp src/io_tools/Source.cpp
        include/catana/io_tools/sources/ObjectContainerSource.hpp src/io_tools/sources/ObjectContainerSource.cpp
        include/catana/io_tools/sources/GadgetSource.hpp src/io_tools/sources/GadgetSource.cpp
        include/catana/io_tools/sources/HDF5Source.hpp src/io_tools/sources/HDF5Source.cpp
        include/catana/io_tools/sources/RawBinarySource.hpp src/io_tools/sources/RawBinarySource.cpp
        include/catana/io_tools/sources/TextSource.hpp src/io_tools/sources/TextSource.cpp)

set(iotools_sinks_SOURCES
        include/catana/io_tools/Sink.hpp
        include/catana/io_tools/sinks/ObjectContainerSink.hpp
        include/catana/io_tools/sinks/PixelizedObjectContainerSink.hpp
        include/catana/io_tools/sinks/HDF5Sink.hpp src/io_tools/sinks/HDF5Sink.cpp
        include/catana/io_tools/sinks/RawBinarySink.hpp src/io_tools/sinks/RawBinarySink.cpp
        include/catana/io_tools/sinks/TextSink.hpp src/io_tools/sinks/TextSink.cpp
        )

set(iotools_filters_SOURCES
        include/catana/io_tools/Filter.hpp src/io_tools/Filter.cpp
        include/catana/io_tools/filters/radial_filters.hpp src/io_tools/filters/radial_filters.cpp
        include/catana/io_tools/filters/angular_filters.hpp src/io_tools/filters/angular_filters.cpp
        )

set(iotools_SOURCES
        ${basictypes_SOURCES}
        ${tools_SOURCES}
        include/catana/io_tools/record_types.hpp src/io_tools/record_type.cpp
        ${iotools_sources_SOURCES}
        ${iotools_sinks_SOURCES}
        ${iotools_filters_SOURCES}
        include/catana/io_tools/FilterStream.hpp src/io_tools/FilterStream.cpp)

#set(window_SOURCES
#        include/catana/Window.hpp src/Window.cpp
#        include/catana/tools/gsl_function_wrapper.hpp)

set(decomposition_SOURCES
        ${returntypes_SOURCES}
        include/catana/decomposition/sfb_decomposition.hpp src/decomposition/sfb_decomposition.cpp
        include/catana/decomposition/sfb_raw.hpp src/decomposition/sfb_raw.cpp
        include/catana/decomposition/sfb_reverse.hpp src/decomposition/sfb_reverse.cpp
        include/catana/decomposition/sfb_reverse_fft.hpp src/decomposition/sfb_reverse_fft.cpp
        src/decomposition/Analyzer.cpp include/catana/decomposition/Analyzer.hpp)

#set(field_SOURCES
#        src/Field.cpp include/catana/Field.hpp
#        include/catana/tools/fftw_field.hpp)

# COMPILE AS STATIC LIBRARY
add_library(iotools STATIC ${iotools_SOURCES})
add_library(besseltools STATIC ${besseltools_SOURCES})
add_library(catana STATIC
#        ${field_SOURCES}
#        ${window_SOURCES}
        ${decomposition_SOURCES})

# LINK TOGETHER
target_link_libraries(besseltools gsl ${CBLAS_LIBRARIES})
target_link_libraries(iotools ${HEALPIX_LIB} hdf5 hdf5_hl boost_system boost_filesystem gsl ${CBLAS_LIBRARIES})
target_link_libraries(catana besseltools iotools fftw3 ${HEALPIX_LIB})
add_dependencies(catana eigen)
add_dependencies(iotools eigen)
