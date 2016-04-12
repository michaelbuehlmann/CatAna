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
set(PYSOURCES
        py_basictypes.cpp py_besseltools.cpp py_io.cpp py_decomposition.cpp
        setup.py CatAna/io.py CatAna/decomp.py CatAna/__init__.py)
#set(DEPENDENCIES eigen besseltools io catana ${PYSOURCES})
set(DEPENDENCIES catana ${PYSOURCES})
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