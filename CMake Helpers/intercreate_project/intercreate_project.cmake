# SPDX-License-Identifier: Apache-2.0
#
# Copyright (c) 2023 Intercreate, Inc.
#
# The ic_project() function sets variables that describe the build
# environment in both CMake and as compile definitions.
#
# Example Usage
#
# All of these variables are prefixed with IC_ (for Intercreate, adjust as
# needed) and are useful for naming build output files and getting build
# information from the system at runtime.
#
# ic_project(
#     # output variables
#     IC_CLIENT_NAME
#     IC_REPO_NAME
#     IC_BOARD_NAME
#     IC_BOARD_REV
#     IC_FW_VERSION
#     IC_BUILD_TYPE
#     IC_GIT_DIRTY
#     IC_GIT_TAG
#     "${out_git_tag_rev}"
#     IC_GIT_HASH
#     IC_GIT_USER_NAME
#     IC_GIT_USER_EMAIL
#     IC_BUILD_DATE
#     IC_BUILD_TIME
#     IC_C_COMPILER_ID
#     IC_C_COMPILER
#     IC_C_COMPILER_VERSION
#     IC_HOST_SYSTEM_NAME
#     IC_HOST_SYSTEM_VERSION
#
#     # keyword arguments
#     CLIENT_NAME "Flumbr"
#     PROJECT_NAME "Dipple Snurp 3"
#     BOARD_NAME FF3
#     BUILD_TYPE debug
#     ARTIFACT_PREFIX prefix
#     # RELEASE_DEPENDS zephyr
# )

macro(_set_default_arg)
    if(NOT DEFINED "_${ARGV1}")
        set("_${ARGV1}" "${ARGV2}")
    endif()
endmacro()

# set() the CMake variable in parent scope and add_compile_definition()
macro(set_def name value)
    if(NOT ${ARGC} EQUAL 2)
        message(
            FATAL_ERROR
            "Invalid arguments to set_def(<name> <value>): (${name} ${value})"
        )
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
function(ic_project
    # output variables
    out_client_name
    out_project_name
    out_board_name
    out_board_rev
    out_fw_version
    out_build_type
    out_git_dirty
    out_git_tag
    out_git_tag_rev
    out_git_hash
    out_git_user_name
    out_git_user_email
    out_build_date
    out_build_time
    out_c_compiler_id
    out_c_compiler
    out_c_compiler_version
    out_host_system_name
    out_host_system_version
)
    set(required_keyword_args
        CLIENT_NAME
        PROJECT_NAME
        BOARD_NAME
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
    cmake_parse_arguments(PARSE_ARGV 19 "" "" "${keyword_args}" "")

    if(_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unparsed arguments: ${_UNPARSED_ARGUMENTS}")
    endif()

    _set_default_arg(_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    _set_default_arg(_ARTIFACT_PREFIX ${CMAKE_PROJECT_NAME})
    _set_default_arg(_DEPENDS_TARGET ${CMAKE_PROJECT_NAME})
    _set_default_arg(_EXTRA_FILES "")

    foreach(KEY ${required_keyword_args})
        if(NOT DEFINED "_${KEY}")
            message(
                FATAL_ERROR
                "Missing keyword argument ${KEY} in call to \
                ${CMAKE_CURRENT_FUNCTION}()"
            )
        endif()
    endforeach()

    set_def("${out_client_name}" ${_CLIENT_NAME})

    project(${_PROJECT_NAME})
    if(NOT ${_PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
        message(FATAL_ERROR "${_PROJECT_NAME} != ${CMAKE_PROJECT_NAME}")
    endif()
    set_def("${out_project_name}" ${CMAKE_PROJECT_NAME})

    set_def("${out_board_name}" ${_BOARD_NAME})
    set_def("${out_board_rev}" "${_BOARD_REV}")

    # Warn about git tag
    execute_process(
        COMMAND git describe
        RESULT_VARIABLE res
        ERROR_QUIET
        OUTPUT_QUIET
    )
    if(res AND NOT res EQUAL 0)
        message(
            WARNING
            "The ${out_fw_version} should be set by git tag but there is no tag"
        )
        set(has_git_tag 0)
    else()
        set(has_git_tag 1)
    endif()

    # Set firmware version
    execute_process(
        COMMAND git describe --dirty=+ --always --abbrev=7
        OUTPUT_VARIABLE git_tag
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set_def("${out_fw_version}" ${git_tag})

    set_def("${out_build_type}" ${_BUILD_TYPE})

    # Set git dirty commit
    string(FIND ${git_tag} + res)
    if(res EQUAL -1)
        set_def("${out_git_dirty}" 0)
    else()
        set_def("${out_git_dirty}" 1)
    endif()
    
    if(has_git_tag)
        # Set the git tag
        string(REPLACE "-" ";" fw_version_list ${fw_version})
        list(GET fw_version_list 0 git_tag)
        set_def("${out_git_tag}" ${git_tag})

        # Set the git tag revision
        list(LENGTH fw_version_list len)
        if (len EQUAL 3)
            list(GET fw_version_list 1 git_tag_rev)
            set_def("${out_git_tag_rev}" ${git_tag_rev})
        else()
            set_def("${out_git_tag_rev}" "")
        endif()
    else()
        set_def("${out_git_tag}" "")
        set_def("${out_git_tag_rev}" "")
    endif()

    # Set short git commit hash
    execute_process(
        COMMAND git describe --dirty=+ --always --abbrev=7 --exclude *
        OUTPUT_VARIABLE git_hash
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set_def("${out_git_hash}" ${git_hash})

    string(TIMESTAMP build_date "%Y%m%d")
    set_def("${out_build_date}" ${build_date})
    string(TIMESTAMP timestamp)
    set_def("${out_build_time}" ${timestamp})

    # Set user info
    execute_process(
        COMMAND git config user.name 
        OUTPUT_VARIABLE user_name
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set_def("${out_git_user_name}" ${user_name})
    execute_process(
        COMMAND git config user.email 
        OUTPUT_VARIABLE user_email
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set_def("${out_git_user_email}" ${user_email})

    # Set build environment info
    set_def("${out_c_compiler_id}" ${CMAKE_C_COMPILER_ID})
    set_def("${out_c_compiler}" ${CMAKE_C_COMPILER})
    set_def("${out_c_compiler_version}" ${CMAKE_C_COMPILER_VERSION})
    set_def("${out_host_system_name}" ${CMAKE_HOST_SYSTEM})
    set_def("${out_host_system_version}" ${CMAKE_HOST_SYSTEM_VERSION})

    set(_NAME )
    set(_cmd "COMMAND ${CMAKE_COMMAND} -E")
    set(_copy "${_cmd} ${_ARTIFACT_PREFIX}")

    add_custom_target(intercreate_output ALL
        ${_cmd} remove_directory intercreate_output
        ${_cmd} make_directory intercreate_output
    )

endfunction()
