cmake_minimum_required(VERSION 3.14)
project(day21_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# 栈队列复习
add_library(day21_stack_queue STATIC
    code/review/stack_queue_summary.cpp
)

# Lambda复习
add_library(day21_lambda STATIC
    code/review/lambda_summary.cpp
)

# EMC++复习
add_library(day21_emcpp STATIC
    code/review/emcpp_review.cpp
)

# LeetCode 155
add_library(day21_lc155 STATIC
    code/leetcode/0155_min_stack/solution.cpp
)

# LeetCode 150
add_library(day21_lc150 STATIC
    code/leetcode/0150_eval_rpn/solution.cpp
)

# 主程序
add_executable(day21_main
    code/main.cpp
    code/review/stack_queue_summary.cpp
    code/review/lambda_summary.cpp
    code/review/emcpp_review.cpp
    code/leetcode/0155_min_stack/solution.cpp
    code/leetcode/0150_eval_rpn/solution.cpp
)

message(STATUS "Day 21 targets:")
message(STATUS "  day21_main        - 运行所有复习")
message(STATUS "  day21_stack_queue - 栈队列复习")
message(STATUS "  day21_lambda      - Lambda复习")
message(STATUS "  day21_emcpp       - EMC++复习")
message(STATUS "  day21_lc155       - LeetCode 155")
message(STATUS "  day21_lc150       - LeetCode 150")
