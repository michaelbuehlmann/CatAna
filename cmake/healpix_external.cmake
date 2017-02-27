set(HEALPIX_PATH ${PROJECT_SOURCE_DIR}/external/healpixmirror/src/cxx)


# COPY ALL HEADER FILES
file(GLOB HEALPIX_CXXSUPPORT_HEADERS
        ${HEALPIX_PATH}/cxxsupport/*.h)
set(HEALPIX_HEADERS
        ${HEALPIX_CXXSUPPORT_HEADERS}
        ${HEALPIX_PATH}/Healpix_cxx/healpix_base.h
        ${HEALPIX_PATH}/Healpix_cxx/healpix_tables.h
        ${HEALPIX_PATH}/Healpix_cxx/healpix_map.h
        ${HEALPIX_PATH}/Healpix_cxx/healpix_map_fitsio.h
        )
set(HEALPIX_INCLUDE_DIR ${PROJECT_BINARY_DIR}/external/healpixmirror/include)
file(COPY ${HEALPIX_HEADERS} DESTINATION ${HEALPIX_INCLUDE_DIR})

# INCLUDE NEEDED HEADER FILES
include_directories(${HEALPIX_INCLUDE_DIR})
include_directories(${HEALPIX_PATH}/c_utils)
file(GLOB HEALPIX_CXXSUPPORT_SOURCES
        ${HEALPIX_PATH}/cxxsupport/*.cc
        ${HEALPIX_PATH}/c_utils/*.c)
set(HEALPIX_SOURCES
        ${HEALPIX_CXXSUPPORT_SOURCES}
        ${HEALPIX_PATH}/Healpix_cxx/healpix_base.cc
        ${HEALPIX_PATH}/Healpix_cxx/healpix_tables.cc
        ${HEALPIX_PATH}/Healpix_cxx/healpix_map.cc
        ${HEALPIX_PATH}/Healpix_cxx/healpix_map_fitsio.cc)

add_library(hplib STATIC ${HEALPIX_SOURCES})
set_target_properties(hplib
        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/catana)
set(HEALPIX_LIBRARIES hplib)