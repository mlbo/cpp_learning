cmake_minimum_required(VERSION 3.14)
project(day17_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# 单调栈演示
add_executable(day17_mono_stack
    code/algorithm/monotonic_stack_demo.cpp
)

# function演示
add_executable(day17_function
    code/cpp11_features/function_demo.cpp
)

# bind演示
add_executable(day17_bind
    code/cpp11_features/bind_demo.cpp
)

# EMC++ Item 34
add_executable(day17_item34
    code/emcpp/item34_lambda_vs_bind.cpp
)

# LeetCode 739
add_executable(day17_lc739
    code/leetcode/0739_daily_temperatures/solution.cpp
)

# LeetCode 496
add_executable(day17_lc496
    code/leetcode/0496_next_greater_element/solution.cpp
)

# 主程序
add_executable(day17_main
    code/main.cpp
    code/algorithm/monotonic_stack_demo.cpp
    code/cpp11_features/function_demo.cpp
    code/cpp11_features/bind_demo.cpp
    code/emcpp/item34_lambda_vs_bind.cpp
    code/leetcode/0739_daily_temperatures/solution.cpp
    code/leetcode/0496_next_greater_element/solution.cpp
)

message(STATUS "Day 17 targets:")
message(STATUS "  day17_main       - 运行所有演示")
message(STATUS "  day17_mono_stack - 单调栈演示")
message(STATUS "  day17_function   - std::function演示")
message(STATUS "  day17_bind       - std::bind演示")
message(STATUS "  day17_item34     - EMC++ Item 34")
message(STATUS "  day17_lc739      - LeetCode 739")
message(STATUS "  day17_lc496      - LeetCode 496")
