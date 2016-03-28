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
