cmake_minimum_required(VERSION 3.16)

project(day_33_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
include(CTest)

add_library(day33_warnings INTERFACE)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(day33_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Werror
    )
endif()

function(add_day33_example target source)
    add_executable(${target} ${source})
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)
    target_link_libraries(${target} PRIVATE day33_warnings Threads::Threads)
    add_test(NAME ${target} COMMAND ${target})
endfunction()

add_day33_example(day33_main code/main.cpp)
add_day33_example(day33_tree_path_demo code/data_structure/tree_path_demo.cpp)
add_day33_example(day33_thread_pool_demo code/cpp11_features/thread_pool_demo.cpp)
add_day33_example(day33_thread_pool_regression
    code/cpp11_features/thread_pool_regression_test.cpp)
add_day33_example(day33_thread_pool_argument_regression
    code/cpp11_features/thread_pool_argument_regression_test.cpp)
add_day33_example(day33_item11_16 code/emcpp/item11_16_misc.cpp)
add_day33_example(day33_lc0257 code/leetcode/0257_binary_tree_paths/solution.cpp)
add_day33_example(day33_lc0113 code/leetcode/0113_path_sum_ii/solution.cpp)
