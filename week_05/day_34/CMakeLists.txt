# ========================================
# Day 34: 并发编程综合 CMake配置
# ========================================

cmake_minimum_required(VERSION 3.14)
project(day_34_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# 数据结构/底层：进程线程模型
add_executable(day34_process_thread_demo
    code/data_structure/process_thread_demo.cpp
)
target_link_libraries(day34_process_thread_demo PRIVATE Threads::Threads)

# C++11特性：并发综合
add_executable(day34_concurrency_demo
    code/cpp11_features/concurrency_demo.cpp
)
target_link_libraries(day34_concurrency_demo PRIVATE Threads::Threads)

# EMC++条款：Item 35-40 复习
add_executable(day34_emcpp_review
    code/emcpp/item35_40_review.cpp
)
target_link_libraries(day34_emcpp_review PRIVATE Threads::Threads)

# LeetCode题目
add_executable(day34_lc0236
    code/leetcode/0236_lca/solution.cpp
    code/leetcode/0236_lca/test.cpp
)

add_executable(day34_lc0105
    code/leetcode/0105_construct_tree/solution.cpp
    code/leetcode/0105_construct_tree/test.cpp
)

# 综合主程序
add_executable(day34_main
    code/main.cpp
    code/data_structure/process_thread_demo.cpp
    code/cpp11_features/concurrency_demo.cpp
    code/emcpp/item35_40_review.cpp
    code/leetcode/0236_lca/solution.cpp
    code/leetcode/0105_construct_tree/solution.cpp
)
target_link_libraries(day34_main PRIVATE Threads::Threads)
