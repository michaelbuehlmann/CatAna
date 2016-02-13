project(catana)

# SOURCE FILES
set(besseltools_SOURCES
        src/besseltools/besseltools.cpp include/catana/besseltools.hpp
        include/catana/tools/gsl_function_wrapper.hpp src/io_tools/sinks/write_hdf5.cpp)

set(basictypes_SOURCES
        include/catana/tools/Object.hpp include/catana/tools/ObjectContainer.hpp
        include/catana/tools/return_types.hpp src/tools/return_types.cpp src/tools/ObjectContainer.cpp)

set(iotools_SOURCES
        ${basictypes_SOURCES}
        include/catana/tools/string_format.hpp
        include/catana/io_tools/record_types.hpp
        include/catana/io_tools/Source.hpp include/catana/io_tools/Sink.hpp
        include/catana/io_tools/sources/read_gadget.hpp src/io_tools/sources/read_gadget.cpp
        include/catana/io_tools/sources/read_hdf5.hpp src/io_tools/sources/read_hdf5.cpp
        include/catana/io_tools/sinks/write_hdf5.hpp src/io_tools/sinks/write_hdf5.cpp
        src/io_tools/record_type.cpp include/catana/io_tools/filters.hpp
        include/catana/io_tools/FilterStream.hpp )

set(window_SOURCES
        include/catana/Window.hpp src/Window.cpp
        include/catana/tools/gsl_function_wrapper.hpp)

set(decomposition_SOURCES
        ${returntypes_SOURCES}
        include/catana/decomposition/sfb_decomposition.hpp src/decomposition/sfb_decomposition.cpp)

set(catalog_SOURCES
        ${basictypes_SOURCES}
        src/Catalog.cpp include/catana/Catalog.hpp
        ${window_SOURCES} ${decomposition_SOURCES})

set(field_SOURCES
        src/Field.cpp include/catana/Field.hpp
        include/catana/tools/fftw_field.hpp
        ${decomposition_SOURCES})

# COMPILE AS STATIC LIBRARY
add_library(iotools STATIC ${iotools_SOURCES})
add_library(besseltools STATIC ${besseltools_SOURCES})
add_library(catana STATIC
        ${catalog_SOURCES}
        ${field_SOURCES}
        ${window_SOURCES}
        ${iotools_SOURCES}
        ${decomposition_SOURCES})

# LINK TOGETHER
target_link_libraries(iotools ${HEALPIX_LIB} hdf5 hdf5_hl boost_system boost_filesystem)
target_link_libraries(besseltools gsl ${CBLAS_LIBRARIES})
target_link_libraries(catana besseltools iotools fftw3 ${HEALPIX_LIB})
add_dependencies(catana eigen)
add_dependencies(iotools eigen)
