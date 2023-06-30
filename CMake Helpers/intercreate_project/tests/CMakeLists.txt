cmake_minimum_required(VERSION 3.20.0)
project(intercreate_project)
include(${CMAKE_SOURCE_DIR}/../intercreate_project.cmake)

intercreate_project(
    CLIENT_NAME "Flumbr"
    PROJECT_NAME "Dipple Snurp 3"
    BOARD FF3
    BUILD_TYPE debug
    ARTIFACT_PREFIX prefix
    RELEASE_DEPENDS zephyr
)
