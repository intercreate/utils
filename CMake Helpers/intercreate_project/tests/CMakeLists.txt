cmake_minimum_required(VERSION 3.20.0)
project(intercreate_project)
include(${CMAKE_SOURCE_DIR}/../intercreate_project.cmake)

ic_project(
    # output variables
    IC_CLIENT_NAME
    IC_PROJECT_NAME
    IC_BOARD_NAME
    IC_BOARD_REV
    IC_FW_VERSION
    IC_BUILD_TYPE
    IC_GIT_DIRTY
    IC_GIT_TAG
    IC_GIT_TAG_REV
    IC_GIT_HASH
    IC_GIT_USER_NAME
    IC_GIT_USER_EMAIL
    IC_BUILD_DATE
    IC_BUILD_TIME
    IC_C_COMPILER_ID
    IC_C_COMPILER
    IC_C_COMPILER_VERSION
    IC_HOST_SYSTEM_NAME
    IC_HOST_SYSTEM_VERSION

    # keyword arguments
    CLIENT_NAME "Flumbr"
    PROJECT_NAME "Dipple Snurp 3"
    BOARD_NAME FF3
    BUILD_TYPE debug
    ARTIFACT_PREFIX prefix
    # RELEASE_DEPENDS zephyr
)
