cmake_minimum_required(VERSION 3.14)
project(day_30_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
include(CTest)

add_library(day30_warnings INTERFACE)
target_compile_options(day30_warnings INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic;-Wconversion;-Wsign-conversion;-Wshadow;-Werror>
)

function(day30_add_test target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)
    target_link_libraries(${target} PRIVATE day30_warnings)
    add_test(NAME ${target} COMMAND ${target})
endfunction()

day30_add_test(day30_tree_traversal
    code/data_structure/tree_traversal_demo.cpp
)

day30_add_test(day30_mutex_demo
    code/cpp11_features/mutex_demo.cpp
)
target_link_libraries(day30_mutex_demo PRIVATE Threads::Threads)

day30_add_test(day30_item36_37
    code/emcpp/item36_37_async.cpp
)
target_link_libraries(day30_item36_37 PRIVATE Threads::Threads)

day30_add_test(day30_lc0094
    code/leetcode/0094_binary_tree_inorder/solution.cpp
    code/leetcode/0094_binary_tree_inorder/test.cpp
)

day30_add_test(day30_lc0102
    code/leetcode/0102_binary_tree_level_order/solution.cpp
    code/leetcode/0102_binary_tree_level_order/test.cpp
)

day30_add_test(day30_main
    code/main.cpp
)
target_link_libraries(day30_main PRIVATE Threads::Threads)
