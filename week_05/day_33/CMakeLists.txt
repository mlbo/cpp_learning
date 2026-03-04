cmake_minimum_required(VERSION 3.14)
project(day_33_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# 数据结构示例
add_executable(day33_tree_path_demo
    code/data_structure/tree_path_demo.cpp
)

# C++11特性示例
add_executable(day33_thread_pool_demo
    code/cpp11_features/thread_pool_demo.cpp
)
target_link_libraries(day33_thread_pool_demo PRIVATE Threads::Threads)

# EMC++条款示例
add_executable(day33_item11_16
    code/emcpp/item11_16_misc.cpp
)

# LeetCode题目
add_executable(day33_lc0257
    code/leetcode/0257_binary_tree_paths/solution.cpp
    code/leetcode/0257_binary_tree_paths/test.cpp
)

add_executable(day33_lc0113
    code/leetcode/0113_path_sum_ii/solution.cpp
    code/leetcode/0113_path_sum_ii/test.cpp
)

# 综合主程序
add_executable(day33_main
    code/main.cpp
    code/data_structure/tree_path_demo.cpp
    code/cpp11_features/thread_pool_demo.cpp
    code/emcpp/item11_16_misc.cpp
    code/leetcode/0257_binary_tree_paths/solution.cpp
    code/leetcode/0113_path_sum_ii/solution.cpp
)
target_link_libraries(day33_main PRIVATE Threads::Threads)
