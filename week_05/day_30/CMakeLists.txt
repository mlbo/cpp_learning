cmake_minimum_required(VERSION 3.14)
project(day_30_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# 数据结构演示
add_library(day30_tree_traversal STATIC
    code/data_structure/tree_traversal_demo.cpp
)

# C++11特性演示
add_library(day30_mutex_demo STATIC
    code/cpp11_features/mutex_demo.cpp
)

# EMC++条款示例
add_library(day30_item36_37 STATIC
    code/emcpp/item36_37_async.cpp
)

# LeetCode题目
add_executable(day30_lc0094
    code/leetcode/0094_binary_tree_inorder/solution.cpp
)

add_executable(day30_lc0102
    code/leetcode/0102_binary_tree_level_order/solution.cpp
)

# 综合主程序
add_executable(day30_main
    code/main.cpp
)
target_link_libraries(day30_main PRIVATE Threads::Threads)
