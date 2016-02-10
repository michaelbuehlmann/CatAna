include(ExternalProject)

####################
# FETCH EIGEN3 LIB #
# TODO: use git submodule
if(NOT EIGEN3_INCLUDE_DIR)
#    set(source_dir ${CMAKE_BINARY_DIR}/external/eigen/cmake)
#    ExternalProject_Add(
#            eigen
#            PREFIX ${source_dir}
#            HG_REPOSITORY https://bitbucket.org/eigen/eigen/
#            CONFIGURE_COMMAND ""
#            BUILD_COMMAND ""
#            INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory "${source_dir}/src/eigen/Eigen" "${CMAKE_BINARY_DIR}/external/eigen/include/Eigen"
#    )
#    set(EIGEN3_INCLUDE_DIR ${CMAKE_BINARY_DIR}/external/eigen/include/)
    ExternalProject_Add(
            eigen
            DOWNLOAD_COMMAND ""
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/external/eigen/Eigen" "${CMAKE_BINARY_DIR}/external/eigen/include/Eigen"
    )
    set(EIGEN3_INCLUDE_DIR ${CMAKE_BINARY_DIR}/external/eigen/include/)
#    set(EIGEN3_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/eigen/include)
endif()

######################
# FETCH Pybind11 LIB #
if(NOT PYBIND11_INCLUDE_DIR)
#    set(source_dir ${CMAKE_BINARY_DIR}/external/pybind11/cmake)
#    ExternalProject_Add(
#            pybind11
#            PREFIX ${source_dir}
#            GIT_REPOSITORY https://github.com/wjakob/pybind11.git
#            CONFIGURE_COMMAND ""
#            BUILD_COMMAND ""
#            INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory "${source_dir}/src/pybind11/include" "${CMAKE_BINARY_DIR}/external/pybind11/include"
#    )
#    set(PYBIND11_INCLUDE_DIR ${CMAKE_BINARY_DIR}/external/pybind11/include/)
    set(PYBIND11_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/pybind11/include/)
endif()

######################
# GOOGLE TESTS
if(NOT GOOGLETEST_DIR)
    set(GOOGLETEST_DIR ${PROJECT_SOURCE_DIR}/external/googletest)
endif()

############################
# GIT versioning
# use tags on git repository
find_package(Git)
if(GIT_FOUND)
    exec_program(
            ${GIT_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}
            ARGS "rev-parse --is-inside-work-tree"
            OUTPUT_VARIABLE GITWORKTREE )
    if( "${GITWORKTREE}" STREQUAL "true" )
        exec_program(
                ${GIT_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}
                ARGS "name-rev --tags --name-only $(git rev-parse HEAD)"
                OUTPUT_VARIABLE GITTAG )
        exec_program(
                ${GIT_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}
                ARGS "rev-parse --short HEAD"
                OUTPUT_VARIABLE GITHASH )
        exec_program(
                ${GIT_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}
                ARGS "diff-index --name-only HEAD"
                OUTPUT_VARIABLE GITDIRTY
                OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(GITDIRTY)
            set (catana_VERSION  "${GITTAG} (${GITHASH}-dirty)")
        else()
            set (catana_VERSION  "${GITTAG} (${GITHASH})")
        endif()
    endif()
endif()
