cmake_minimum_required(VERSION 3.14)
project(day_31_bst LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Werror
    )
endif()

find_package(Threads REQUIRED)
include(CTest)

function(day31_add_test target source)
    add_executable(${target} ${source})
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)
    add_test(NAME ${target} COMMAND ${target})
    set_tests_properties(${target} PROPERTIES TIMEOUT 10)
endfunction()

day31_add_test(day31_main code/main.cpp)
day31_add_test(day31_bst_demo code/data_structure/bst_demo.cpp)
day31_add_test(day31_condition_variable code/cpp11_features/condition_variable_demo.cpp)
target_link_libraries(day31_condition_variable PRIVATE Threads::Threads)

day31_add_test(day31_item38 code/emcpp/item38_thread_handle.cpp)
target_link_libraries(day31_item38 PRIVATE Threads::Threads)

day31_add_test(day31_lc0098 code/leetcode/0098_validate_bst/solution.cpp)
day31_add_test(day31_lc0700 code/leetcode/0700_search_bst/solution.cpp)
