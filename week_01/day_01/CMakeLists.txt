cmake_minimum_required(VERSION 3.10)
project(day_01_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# 数据结构示例
add_library(day01_complexity STATIC
    code/data_structure/complexity.cpp
)

# C++11特性示例
add_library(day01_auto_demo STATIC
    code/cpp11_features/auto_demo.cpp
)

# EMC++条款示例
add_library(day01_emcpp STATIC
    code/emcpp/type_deduction_items.cpp
)

# LeetCode题目
add_library(day01_lc0001 STATIC
    code/leetcode/0001_two_sum/solution.cpp
    code/leetcode/0001_two_sum/test.cpp
)

add_library(day01_lc0167 STATIC
    code/leetcode/0167_two_sum_ii/solution.cpp
    code/leetcode/0167_two_sum_ii/test.cpp
)

# 主程序
add_executable(day01_main 
    code/main.cpp
    code/data_structure/complexity.cpp
    code/cpp11_features/auto_demo.cpp
    code/emcpp/type_deduction_items.cpp
    code/leetcode/0001_two_sum/solution.cpp
    code/leetcode/0001_two_sum/test.cpp
    code/leetcode/0167_two_sum_ii/solution.cpp
    code/leetcode/0167_two_sum_ii/test.cpp
)
