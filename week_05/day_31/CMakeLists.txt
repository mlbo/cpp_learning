cmake_minimum_required(VERSION 3.14)
project(day_31_bst CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# 数据结构演示
add_executable(day31_bst_demo
    code/data_structure/bst_demo.cpp
)

# C++11特性演示
add_executable(day31_condition_variable
    code/cpp11_features/condition_variable_demo.cpp
)
target_link_libraries(day31_condition_variable PRIVATE Threads::Threads)

# EMC++条款示例
add_executable(day31_item38
    code/emcpp/item38_thread_handle.cpp
)
target_link_libraries(day31_item38 PRIVATE Threads::Threads)

# LeetCode题目
add_executable(day31_lc0098
    code/leetcode/0098_validate_bst/solution.cpp
)

add_executable(day31_lc0700
    code/leetcode/0700_search_bst/solution.cpp
)

# 综合主程序
add_executable(day31_main
    code/main.cpp
    code/data_structure/bst_demo.cpp
    code/cpp11_features/condition_variable_demo.cpp
    code/emcpp/item38_thread_handle.cpp
)
target_link_libraries(day31_main PRIVATE Threads::Threads)
