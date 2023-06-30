# SPDX-License-Identifier: Apache-2.0
#
# Copyright (c) 2023 Intercreate, Inc.
#
# The intercreate_project() function sets variables that describe the build
# environment in both CMake and as compile definitions.

# All of these variables are prefixed with IC_ (for Intercreate, adjust as
# needed) and are useful for naming build output files and getting build
# information from the system at runtime.
#

macro(_set_default_arg)
    if(NOT DEFINED "_${ARGV1}")
        set("_${ARGV1}" "${ARGV2}")
    endif()
endmacro()

# set() the CMake variable in local and parent scope and add_compile_definition()
macro(set_def name value)
    if(NOT ${ARGC} EQUAL 2)
        message(FATAL_ERROR "Invalid arguments to set_def(<name> <value>): (${name} ${value})")
    endif()
    if(DEFINED name)
        message(FATAL_ERROR "${name} is already defined!")
    endif()
    set(${name} ${value})
    set(${name} ${value} PARENT_SCOPE)
    add_compile_definitions(${name} ${value})
    message(STATUS "${name} = ${value}")
endmacro()

# Create well-named build output files, folder, and archives
function(intercreate_project)
    set(required_keyword_args
        CLIENT_NAME
        PROJECT_NAME
        BOARD
    )
    set(optional_keyword_args
        BUILD_TYPE
        ARTIFACT_PREFIX
        DEPENDS_TARGET
        EXTRA_FILES
    )
    list(APPEND keyword_args
        ${required_keyword_args}
        ${optional_keyword_args}
    )
    cmake_parse_arguments(PARSE_ARGV 0 "" "" "${keyword_args}" "")

    _set_default_arg(_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    _set_default_arg(_ARTIFACT_PREFIX ${CMAKE_PROJECT_NAME})
    _set_default_arg(_DEPENDS_TARGET ${CMAKE_PROJECT_NAME})
    _set_default_arg(_EXTRA_FILES "")

    foreach(KEY ${required_keyword_args})
        if(NOT DEFINED "_${KEY}")
            message(FATAL_ERROR "Missing keyword argument ${KEY} in call to ${CMAKE_CURRENT_FUNCTION}()")
        endif()
    endforeach()

    set_def(IC_CLIENT_NAME ${_CLIENT_NAME})

    project(${_PROJECT_NAME})
    if(NOT ${_PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
        message(FATAL_ERROR "${_PROJECT_NAME} != ${CMAKE_PROJECT_NAME}")
    endif()
    set_def(IC_PROJECT_NAME ${CMAKE_PROJECT_NAME})

    set_def(IC_BOARD ${_BOARD})

    # Warn about git tag
    execute_process(
        COMMAND git describe
        RESULT_VARIABLE res
        ERROR_QUIET
        OUTPUT_QUIET
    )
    if(res AND NOT res EQUAL 0)
        message(WARNING "The IC_FW_VERSION should be set by git tag semver but there is no tag")
        set(has_git_tag 0)
    else()
        set(has_git_tag 1)
    endif()

    # Set firmware version
    execute_process(
        COMMAND git describe --dirty=+ --always --abbrev=7
        OUTPUT_VARIABLE fw_version
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set_def(IC_FW_VERSION ${fw_version})

    # Set git dirty commit
    string(FIND ${fw_version} + res)
    if(res EQUAL -1)
        set_def(IC_GIT_DIRTY 0)
    else()
        set_def(IC_GIT_DIRTY 1)
    endif()
    
    if(has_git_tag)
        # Set the git tag
        string(REPLACE "-" ";" fw_version_list ${fw_version})
        list(GET fw_version_list 0 git_tag)
        set_def(IC_GIT_TAG ${git_tag})

        # Set the git tag revision
        list(LENGTH fw_version_list len)
        if (len EQUAL 3)
            list(GET fw_version_list 1 git_tag_rev)
            set_def(IC_GIT_TAG_REV ${git_tag_rev})
        else()
            set_def(IC_GIT_TAG_REV "")
        endif()
    else()
        set_def(IC_GIT_TAG "")
        set_def(IC_GIT_TAG_REV "")
    endif()

    # Set short git commit hash
    execute_process(
        COMMAND git describe --dirty=+ --always --abbrev=7 --exclude *
        OUTPUT_VARIABLE git_hash
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set_def(IC_GIT_HASH ${git_hash})

    # Set build date
    string(TIMESTAMP build_date "%Y%m%d")
    set_def(IC_BUILD_DATE ${build_date})

    set_def(IC_BUILD_TYPE ${_BUILD_TYPE})

    # Set user info
    execute_process(
        COMMAND git config user.name 
        OUTPUT_VARIABLE user_name
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set_def(IC_USER_NAME ${user_name})
    execute_process(
        COMMAND git config user.email 
        OUTPUT_VARIABLE user_email
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set_def(IC_USER_EMAIL ${user_email})

    # Set build environment info
    set_def(IC_C_COMPILER_ID ${CMAKE_C_COMPILER_ID})
    set_def(IC_C_COMPILER ${CMAKE_C_COMPILER})
    set_def(IC_C_COMPILER_VERSION ${CMAKE_C_COMPILER_VERSION})
    set_def(IC_HOST_SYSTEM_NAME ${CMAKE_HOST_SYSTEM})
    set_def(IC_HOST_SYSTEM_VERSION ${CMAKE_HOST_SYSTEM_VERSION})
    string(TIMESTAMP timestamp)
    set_def(IC_BUILD_TIME ${timestamp})

    set(_NAME )
    set(_cmd "COMMAND ${CMAKE_COMMAND} -E")
    set(_copy "${_cmd} ${_ARTIFACT_PREFIX}")

    add_custom_target(intercreate_output ALL
        ${_cmd} remove_directory intercreate_output
        ${_cmd} make_directory intercreate_output
    )

endfunction()
