include_directories(../catana/include)
include_directories(${PYTHON_INCLUDE_DIR})

# Use this later, when setup.py works...
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CatAna)
#configure_file(setup.py setup.py)
#configure_file(CatAna/__init__.py CatAna/__init__.py)
#configure_file(MANIFEST.in MANIFEST.in)
#configure_file(README.rst README.rst)

# Just write files to top_dir/build...
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/python/CatAna)
configure_file(CatAna/__init__.py ${CMAKE_SOURCE_DIR}/build/python/CatAna/__init__.py)

# The actual python library
add_library(py_besseltools SHARED CatAna/py_besseltools.cpp)
set_target_properties(py_besseltools PROPERTIES OUTPUT_NAME besseltools)
target_link_libraries(py_besseltools besseltools)
target_link_libraries(py_besseltools cblas)

add_library(py_basictypes SHARED CatAna/py_basictypes.cpp)
set_target_properties(py_basictypes PROPERTIES OUTPUT_NAME basictypes)
target_link_libraries(py_basictypes iotools)
add_dependencies(py_basictypes eigen)

add_library(py_iotools SHARED CatAna/py_iotools.cpp)
set_target_properties(py_iotools PROPERTIES OUTPUT_NAME io_core)
target_link_libraries(py_iotools iotools)
add_dependencies(py_iotools eigen)

add_library(py_decomposition SHARED CatAna/py_decomposition.cpp)
set_target_properties(py_decomposition PROPERTIES OUTPUT_NAME decomp_core)
target_link_libraries(py_decomposition iotools catana)
add_dependencies(py_decomposition eigen)

foreach(LIB py_besseltools py_basictypes py_iotools py_decomposition)
    set_target_properties(${LIB} PROPERTIES PREFIX "")  # No 'libname.so' but 'name.so'

    if (UNIX)
        # It's quite common to have multiple copies of the same Python version
        # installed on one's system. E.g.: one copy from the OS and another copy
        # that's statically linked into an application like Blender or Maya.
        # If we link our plugin library against the OS Python here and import it
        # into Blender or Maya later on, this will cause segfaults when multiple
        # conflicting Python instances are active at the same time (even when they
        # are of the same version).

        # Windows is not affected by this issue since it handles DLL imports
        # differently. The solution for Linux and Mac OS is simple: we just don't
        # link against the Python library. The resulting shared library will have
        # missing symbols, but that's perfectly fine -- they will be resolved at
        # import time.

        # .SO file extension on Linux/Mac OS
        set_target_properties(${LIB} PROPERTIES SUFFIX ".so")

        # Strip unnecessary sections of the binary on Linux/Mac OS
        if(APPLE)
            set_target_properties(${LIB} PROPERTIES MACOSX_RPATH ".")
            set_target_properties(${LIB} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup ")
#            if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
#                add_custom_command(TARGET ${LIB} POST_BUILD COMMAND strip -u -r ${PROJECT_BINARY_DIR}/python/CatAna/${LIB}.so)
#            endif()
        else()
#            if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
#                add_custom_command(TARGET ${LIB} POST_BUILD COMMAND strip ${PROJECT_BINARY_DIR}/python/CatAna/${LIB}.so)
#            endif()
        endif()
    endif()
endforeach()

add_custom_target(python_libraries
        DEPENDS py_besseltools py_basictypes py_iotools py_decomposition)