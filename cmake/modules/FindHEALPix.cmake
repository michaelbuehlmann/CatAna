# Find HEALPix
#
# HEALPIX package includes following libraries:
# healpix_cxx cxxsupport sharp fftpack c_utils  /*Order of these libraries is very important to resolve dependencies*/
#
# Once run this will define:
#
# HEALPIX_FOUND:       healpixsystem has HEALPIX lib
# HEALPIX_LIBRARIES:   full path to the HEALPIX package libraries
# HEALPIX_INCLUDE_DIR: where to find headers
#
# Set HEALPIX_INCLUDEPATH and HEALPIX_LIBPATH

SET(HEALPIX_FOUND FALSE)

FIND_PATH(HEALPIX_INCLUDE_DIR
        NAMES
        healpix_base.h
        PATHS
        ${HEALPIX_INCLUDEPATH}
        /usr/include
        /usr/local/include
        )

FIND_PATH(HEALPIX_LIB_DIR
        NAMES
        libhealpix_cxx.a
        PATHS
        ${HEALPIX_LIBPATH}
        /usr/lib
        /usr/local/lib
        )

IF(HEALPIX_INCLUDE_DIR AND HEALPIX_LIB_DIR)
    SET(HEALPIX_FOUND TRUE)
    MESSAGE(STATUS "HealPix Found!")
    MESSAGE(STATUS "HEALPIX_INCLUDE_DIR=${HEALPIX_INCLUDE_DIR}")
    MESSAGE(STATUS "HEALPIX_LIB_DIR=${HEALPIX_INCLUDE_DIR}")


    FIND_LIBRARY(HEALPIX_LIBRARIES
            NAMES
            libhealpix_cxx.a
            healpix_cxx
            PATHS
            ${HEALPIX_LIB_DIR}
            NO_DEFAULT_PATH
            )
    FIND_LIBRARY(HEALPIX_CXXSUPPORT_LIBRARIES
            NAMES
            libcxxsupport.a
            cxxsupport
            PATHS
            ${HEALPIX_LIB_DIR}
            NO_DEFAULT_PATH
            )
    FIND_LIBRARY(HEALPIX_SHARP_LIBRARIES
            NAMES
            libsharp.a
            sharp
            PATHS
            ${HEALPIX_LIB_DIR}
            NO_DEFAULT_PATH
            )
    FIND_LIBRARY(HEALPIX_FFTPACK_LIBRARIES
            NAMES
            libfftpack.a
            fftpack
            PATHS
            ${HEALPIX_LIB_DIR}
            NO_DEFAULT_PATH
            )
    FIND_LIBRARY(HEALPIX_CUTILS_LIBRARIES
            NAMES
            libc_utils.a
            c_utils
            PATHS
            ${HEALPIX_LIB_DIR}
            NO_DEFAULT_PATH
            )

    IF(HEALPIX_LIBRARIES AND HEALPIX_CXXSUPPORT_LIBRARIES AND HEALPIX_SHARP_LIBRARIES AND HEALPIX_FFTPACK_LIBRARIES AND HEALPIX_CUTILS_LIBRARIES)
        SET(HEALPIX_LIBRARIES ${HEALPIX_LIBRARIES}
                ${HEALPIX_CXXSUPPORT_LIBRARIES}
                ${HEALPIX_SHARP_LIBRARIES}
                ${HEALPIX_FFTPACK_LIBRARIES}
                ${HEALPIX_CUTILS_LIBRARIES})
    ENDIF()
    MESSAGE(STATUS "HEALPIX_LIBRARIES=${HEALPIX_LIBRARIES}")
ELSE()
    MESSAGE(WARNING "Could not find HEALPix")
    MESSAGE(STATUS "HEALPIX_INCLUDE_DIR=${HEALPIX_INCLUDE_DIR}")
    MESSAGE(STATUS "HEALPIX_LIBRARIES=${HEALPIX_LIBRARIES}")
ENDIF()


MARK_AS_ADVANCED(
        HEALPIX_INCLUDE_DIR
        HEALPIX_LIBRARIES
        HEALPIX_FOUND
)

IF(NOT HEALPIX_FOUND AND HEALPIX_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "HEALPix library not found. Please specify library location: HEALPIX_INCLUDEPATH, HEALPIX_LIBPATH")
ENDIF()