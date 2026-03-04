cmake_minimum_required(VERSION 3.14)
project(day_33_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# 数据结构示例
add_library(day33_tree_path_demo STATIC
    code/data_structure/tree_path_demo.cpp
)

# C++11特性示例
add_library(day33_thread_pool_demo STATIC
    code/cpp11_features/thread_pool_demo.cpp
)

# EMC++条款示例
add_library(day33_item11_16 STATIC
    code/emcpp/item11_16_misc.cpp
)

# LeetCode题目
add_library(day33_lc0257 STATIC
    code/leetcode/0257_binary_tree_paths/solution.cpp
)

add_library(day33_lc0113 STATIC
    code/leetcode/0113_path_sum_ii/solution.cpp
)

# 综合主程序
add_executable(day33_main
    code/main.cpp
)
target_link_libraries(day33_main PRIVATE Threads::Threads)
