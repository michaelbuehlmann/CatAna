include_directories(../catana/include)
include_directories(${PYTHON_INCLUDE_DIR})

get_property(LIB_LOCATION_ TARGET catana PROPERTY LOCATION)
get_filename_component(LIB_LOCATION ${LIB_LOCATION_} DIRECTORY)
message(STATUS "libraries were build int ${LIB_LOCATION}")

set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py)
set(SETUP_PY ${CMAKE_CURRENT_BINARY_DIR}/setup.py)

configure_file(${SETUP_PY_IN} ${SETUP_PY})
configure_file(CatAna/__init__.py  ${CMAKE_CURRENT_BINARY_DIR}/CatAna/__init__.py)
configure_file(CatAna/io.py        ${CMAKE_CURRENT_BINARY_DIR}/CatAna/io.py)
configure_file(CatAna/decomp.py    ${CMAKE_CURRENT_BINARY_DIR}/CatAna/decomp.py)

### NEW SETUP.PY COMPILATION ###
set(DEPENDENCIES eigen besseltools io catana)
set(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/build/timestamp)
add_custom_command(
        OUTPUT ${OUTPUT}
        COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} build
        COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
        DEPENDS ${DEPENDENCIES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_target(python ALL DEPENDS ${OUTPUT})
install(CODE "execute_process(
        COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})"
        )

add_custom_target(python_install
        make install
        DEPENDS python
        )


### OLD CMAKE COMPILATION ###
#
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/python/CatAna)
#
# The actual python library
#add_library(py_besseltools SHARED CatAna/py_besseltools.cpp)
#set_target_properties(py_besseltools PROPERTIES OUTPUT_NAME besseltools)
#target_link_libraries(py_besseltools besseltools)
#target_link_libraries(py_besseltools cblas)
#
#add_library(py_basictypes SHARED CatAna/py_basictypes.cpp)
#set_target_properties(py_basictypes PROPERTIES OUTPUT_NAME basictypes)
#target_link_libraries(py_basictypes io)
#add_dependencies(py_basictypes eigen)
#
#add_library(py_io SHARED CatAna/py_io.cpp)
#set_target_properties(py_io PROPERTIES OUTPUT_NAME io_core)
#target_link_libraries(py_io io)
#add_dependencies(py_io eigen)
#
#add_library(py_decomposition SHARED CatAna/py_decomposition.cpp)
#set_target_properties(py_decomposition PROPERTIES OUTPUT_NAME decomp_core)
#target_link_libraries(py_decomposition io catana)
#add_dependencies(py_decomposition eigen)
#
#foreach(LIB py_besseltools py_basictypes py_io py_decomposition)
#    set_target_properties(${LIB} PROPERTIES PREFIX "")  # No 'libname.so' but 'name.so'
#
#    if (UNIX)
#        # It's quite common to have multiple copies of the same Python version
#        # installed on one's system. E.g.: one copy from the OS and another copy
#        # that's statically linked into an application like Blender or Maya.
#        # If we link our plugin library against the OS Python here and import it
#        # into Blender or Maya later on, this will cause segfaults when multiple
#        # conflicting Python instances are active at the same time (even when they
#        # are of the same version).
#
#        # Windows is not affected by this issue since it handles DLL imports
#        # differently. The solution for Linux and Mac OS is simple: we just don't
#        # link against the Python library. The resulting shared library will have
#        # missing symbols, but that's perfectly fine -- they will be resolved at
#        # import time.
#
#        # .SO file extension on Linux/Mac OS
#        set_target_properties(${LIB} PROPERTIES SUFFIX ".so")
#
#        # Strip unnecessary sections of the binary on Linux/Mac OS
#        if(APPLE)
#            set_target_properties(${LIB} PROPERTIES MACOSX_RPATH ".")
#            set_target_properties(${LIB} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup ")
##            if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
##                add_custom_command(TARGET ${LIB} POST_BUILD COMMAND strip -u -r ${PROJECT_BINARY_DIR}/python/CatAna/${LIB}.so)
##            endif()
#        else()
##            if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
##                add_custom_command(TARGET ${LIB} POST_BUILD COMMAND strip ${PROJECT_BINARY_DIR}/python/CatAna/${LIB}.so)
##            endif()
#        endif()
#    endif()
#endforeach()
#
#add_custom_target(python_libraries
#        DEPENDS py_besseltools py_basictypes py_io py_decomposition)