if(NOT PYTHON)
    find_program(PYTHON "python")
endif()

if (PYTHON)
#
    set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
    set(SETUP_PY "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
    set(DEPS "${CMAKE_CURRENT_SOURCE_DIR}/pycatana.pyx")
    set(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")

    get_property(include_dir DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    set(library_dir ${CMAKE_BINARY_DIR}/catana)

    configure_file(pycatana.pyx pycatana.pyx)
    configure_file(${SETUP_PY_IN} ${SETUP_PY})

    add_custom_command(OUTPUT ${OUTPUT}
            COMMAND ${PYTHON} ${SETUP_PY} build
            COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
            DEPENDS "pycatana.pyx" "setup.py.in")

    add_custom_target(pycatana ALL DEPENDS ${OUTPUT})
    add_dependencies(pycatana besseltools)

    install(CODE "execute_process(
        COMMAND ${PYTHON} ${SETUP_PY} install)"
            )
endif ()