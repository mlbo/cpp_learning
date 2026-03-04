cmake_minimum_required(VERSION 3.14)
project(day15_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# 主程序（包含所有演示）
add_executable(day15_main
    code/main.cpp
    code/data_structure/stack_demo.cpp
    code/cpp11_features/lambda_demo.cpp
    code/cpp11_features/lambda_capture_demo.cpp
    code/emcpp/item31_avoid_default_capture.cpp
    code/leetcode/0020_valid_parentheses/solution.cpp
    code/leetcode/1047_remove_adjacent_duplicates/solution.cpp
)

# 单独的可执行文件
add_library(day15_stack_demo STATIC
    code/data_structure/stack_demo.cpp
)

add_library(day15_lambda_demo STATIC
    code/cpp11_features/lambda_demo.cpp
)

add_library(day15_item31 STATIC
    code/emcpp/item31_avoid_default_capture.cpp
)

add_library(day15_lc20 STATIC
    code/leetcode/0020_valid_parentheses/solution.cpp
)

add_library(day15_lc1047 STATIC
    code/leetcode/1047_remove_adjacent_duplicates/solution.cpp
)

message(STATUS "Day 15 targets:")
message(STATUS "  day15_main        - 运行所有演示")
message(STATUS "  day15_stack_demo  - 栈演示")
message(STATUS "  day15_lambda_demo - Lambda演示")
message(STATUS "  day15_item31      - EMC++ Item 31")
message(STATUS "  day15_lc20        - LeetCode 20")
message(STATUS "  day15_lc1047      - LeetCode 1047")
