# SPDX-License-Identifier: Apache-2.0
# 
# Copyright (c) 2022-2023 Intercreate, Inc.
# Author: Dave S Desrochers <dave@intercreate.io>
#
# @file Target to create build information header
#

execute_process(
    COMMAND git describe --always --dirty=*
    OUTPUT_VARIABLE GITVERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND whoami
    OUTPUT_VARIABLE WHOAMI
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(STRIP "${GITVERSION}" GITVERSION)
string(TIMESTAMP COMPILE_DATE "%Y-%m-%dT%H:%M:%S")
string(TIMESTAMP COMPILE_UNIXTIME "%s")

set(VERSION_H "
/* DO NOT MODIFY. This is an automatically generated file. */
#pragma once
#define APP_VERSION  \"${GITVERSION}\"
#define COMPILE_TIME \"${COMPILE_DATE}\"
#define COMPILE_UNIXTIME ${COMPILE_UNIXTIME}
#define COMPILE_OS   \"${HOST_SYSTEM}\"
#define APP_AUTHOR   \"${WHOAMI}\"
")

if(EXISTS ${FILENAME})
    file(READ ${FILENAME} VERSION_)
else()
    set(VERSION_ "")
endif()

if(NOT "${VERSION_H}" STREQUAL "${VERSION_}")
    file(WRITE ${FILENAME} "${VERSION_H}")
endif()
