# SPDX-License-Identifier: Apache-2.0
#
# Copyright (c) 2023 Intercreate, Inc.
# Author: J.P. Hutchins <jp@intercreate.io>
# 
# Base CMake for tests.  Put it in you test folder an include at the top of each tests'
# CMakeLists.txt, e.g.:
#
#   cmake_minimum_required(VERSION 3.20.0)
#   include($ENV{ENVR_ROOT}/tests/ztest.cmake)
#
# Without modification, this cmake requires envr (https://github.com/JPhutchins/envr)
# for environment variable management.  Requires the following environment variable definitions,
# typically defined in an envr-default file.
#
#   ENVR_ROOT : usually the root of the repository (or automatically the location of envr)
#   PROJECT_ZEPHYR_VERSION : the version of Zephyr to use
#   ZEPHYR_BASE : zephyr location
#
# It is assumed that your Zephyr application is in the directory app/ and tests are in a folder
# called tests/.
#
# tests/test_on_target.conf is a base configuration in common for all on-target tests
#

if(${BOARD} STREQUAL $ENV{BOARD})
    list(APPEND CONF_FILE
        $ENV{ENVR_ROOT}/tests/test_on_target.conf  # base conf, only for on-target tests
    )
endif()

list(APPEND CONF_FILE
    ${CMAKE_SOURCE_DIR}/prj.conf  # test suite conf
)

list(APPEND DTS_ROOT
    $ENV{ENVR_ROOT}/app
)

list(APPEND BOARD_ROOT
    $ENV{ENVR_ROOT}/app
)

add_subdirectory($ENV{ENVR_ROOT}/patches patches)

find_package(Zephyr $ENV{PROJECT_ZEPHYR_VERSION} EXACT REQUIRED HINTS $ENV{ZEPHYR_BASE})

if(${BOARD} STREQUAL $ENV{BOARD})
    target_sources(app PRIVATE
        # add common dependencies for on-target ZTESTs
    )

    # copy the zephyr.elf to the tests/build folder for debugging
    add_custom_command(
        COMMAND ${CMAKE_COMMAND} -E copy zephyr/zephyr.elf $ENV{ENVR_ROOT}/tests/build/zephyr.elf
        OUTPUT $ENV{ENVR_ROOT}/tests/build/zephyr.elf
        DEPENDS zephyr_final
    )
    add_custom_target(copy_elf_to_tests_folder
        ALL
        DEPENDS $ENV{ENVR_ROOT}/tests/build/zephyr.elf
    )
endif()

# add common include paths for ZTESTs
target_include_directories(app PRIVATE
    $ENV{ENVR_ROOT}/app/src
)
