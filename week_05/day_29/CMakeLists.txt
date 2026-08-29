cmake_minimum_required(VERSION 3.14)
project(day_29_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
include(CTest)

add_library(day29_warnings INTERFACE)
target_compile_options(day29_warnings INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic;-Wconversion;-Wsign-conversion;-Wshadow;-Werror>
)

function(day29_add_test target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)
    target_link_libraries(${target} PRIVATE day29_warnings)
    add_test(NAME ${target} COMMAND ${target})
endfunction()

day29_add_test(day29_binary_tree_demo
    code/data_structure/binary_tree_demo.cpp
)

day29_add_test(day29_thread_demo
    code/cpp11_features/thread_demo.cpp
)
target_link_libraries(day29_thread_demo PRIVATE Threads::Threads)

day29_add_test(day29_thread_group_regression
    code/cpp11_features/thread_group_regression_test.cpp
)
target_link_libraries(day29_thread_group_regression PRIVATE Threads::Threads)

day29_add_test(day29_item35
    code/emcpp/item35_task_based.cpp
)
target_link_libraries(day29_item35 PRIVATE Threads::Threads)

day29_add_test(day29_lc0144
    code/leetcode/0144_binary_tree_preorder/solution.cpp
    code/leetcode/0144_binary_tree_preorder/test.cpp
)

day29_add_test(day29_lc0145
    code/leetcode/0145_binary_tree_postorder/solution.cpp
    code/leetcode/0145_binary_tree_postorder/test.cpp
)

day29_add_test(day29_main
    code/main.cpp
)
target_link_libraries(day29_main PRIVATE Threads::Threads)
