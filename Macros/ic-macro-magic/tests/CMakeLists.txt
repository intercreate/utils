cmake_minimum_required(VERSION 3.20)
project(tests)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG 1b9199ee380e203603b6649df9510db9cab147d9
)
FetchContent_MakeAvailable(unity)
include_directories(${unity_SOURCE_DIR}/src)

file(GLOB tests RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "test_*")
message(STATUS "Collected tests [ ${tests} ]")
foreach(test ${tests})
    project(${test})    
    file(GLOB_RECURSE src_files ${CMAKE_CURRENT_SOURCE_DIR}/${test}/*.c)
    message(STATUS "${test} src_files [ ${src_files} ]")
    add_executable(${test} ${src_files})
    target_link_libraries(${test} unity)
    add_test(${test} ${test})
endforeach()
