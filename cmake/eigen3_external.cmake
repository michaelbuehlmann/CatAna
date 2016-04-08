if(NOT EIGEN3_INCLUDE_DIR)
    ExternalProject_Add(
            eigen
            DOWNLOAD_COMMAND ""
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/external/eigen/Eigen" "${CMAKE_BINARY_DIR}/external/eigen/include/Eigen"
    )
    set(EIGEN3_INCLUDE_DIR ${CMAKE_BINARY_DIR}/external/eigen/include/)
endif()