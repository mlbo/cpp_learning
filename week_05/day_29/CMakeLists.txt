cmake_minimum_required(VERSION 3.14)
project(day_29_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找线程库
find_package(Threads REQUIRED)

# 数据结构示例
add_library(day29_binary_tree_demo STATIC
    code/data_structure/binary_tree_demo.cpp
)

# C++11特性示例
add_executable(day29_thread_demo
    code/cpp11_features/thread_demo.cpp
)
target_link_libraries(day29_thread_demo PRIVATE Threads::Threads)

# EMC++条款示例
add_executable(day29_item35
    code/emcpp/item35_task_based.cpp
)
target_link_libraries(day29_item35 PRIVATE Threads::Threads)

# LeetCode题目
add_executable(day29_lc0144
    code/leetcode/0144_binary_tree_preorder/solution.cpp
    code/leetcode/0144_binary_tree_preorder/test.cpp
)

add_executable(day29_lc0145
    code/leetcode/0145_binary_tree_postorder/solution.cpp
    code/leetcode/0145_binary_tree_postorder/test.cpp
)

# 综合主程序
add_executable(day29_main
    code/main.cpp
)
target_link_libraries(day29_main PRIVATE Threads::Threads)
