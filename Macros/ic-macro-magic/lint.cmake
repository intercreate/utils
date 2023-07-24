file(GLOB_RECURSE files_to_format 
    ${CMAKE_CURRENT_LIST_DIR}/include/ic_macro_magic.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ic_macro_magic/ic_macro_magic_internal.h
    ${CMAKE_CURRENT_LIST_DIR}/tests/test_*/*.c
)

if("${CMAKE_ARGV3}" STREQUAL "format")
    message("Running clang-format in-place")
    foreach(file ${files_to_format})
        execute_process(
            COMMAND clang-format "-i" "${file}"
            RESULT_VARIABLE res 
        )
        if(NOT ${res} STREQUAL "0")
            message(FATAL_ERROR "clang-format error")
        endif()
    endforeach()
endif()

foreach(file ${files_to_format})
    execute_process(
        COMMAND clang-format "--dry-run" "--Werror" "${file}"
        RESULT_VARIABLE res 
    )
    if(NOT ${res} STREQUAL "0")
        message(FATAL_ERROR "clang-format error")
    endif()
endforeach()
message("clang-format passed")
