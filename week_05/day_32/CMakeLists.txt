cmake_minimum_required(VERSION 3.14)
project(day_32_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# 数据结构示例
add_library(day32_dfs_demo STATIC
    code/data_structure/dfs_demo.cpp
)

# C++11特性示例
add_library(day32_atomic_demo STATIC
    code/cpp11_features/atomic_demo.cpp
)

# EMC++条款示例
add_library(day32_item39_40 STATIC
    code/emcpp/item39_40_concurrent.cpp
)

# LeetCode题目
add_executable(day32_lc0104
    code/leetcode/0104_maximum_depth/solution.cpp
)

add_executable(day32_lc0111
    code/leetcode/0111_minimum_depth/solution.cpp
)

# 综合主程序
add_executable(day32_main
    code/main.cpp
)
target_link_libraries(day32_main PRIVATE Threads::Threads)
