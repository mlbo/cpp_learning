cmake_minimum_required(VERSION 3.14)
project(day18_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# 函数调用栈演示
add_library(day18_call_stack STATIC
    code/low_level/call_stack_demo.cpp
)

# 栈帧演示
add_library(day18_stack_frame STATIC
    code/low_level/stack_frame.cpp
)

# enum class演示
add_library(day18_enum_class STATIC
    code/cpp11_features/enum_class_demo.cpp
)

# EMC++ Item 10演示
add_library(day18_item10 STATIC
    code/emcpp/item10_enum_class.cpp
)

# LeetCode 84
add_library(day18_lc84 STATIC
    code/leetcode/0084_largest_rectangle/solution.cpp
)

# LeetCode 42
add_library(day18_lc42 STATIC
    code/leetcode/0042_trapping_rain_water/solution.cpp
)

# 主程序
add_executable(day18_main
    code/main.cpp
    code/low_level/call_stack_demo.cpp
    code/low_level/stack_frame.cpp
    code/cpp11_features/enum_class_demo.cpp
    code/emcpp/item10_enum_class.cpp
    code/leetcode/0084_largest_rectangle/solution.cpp
    code/leetcode/0042_trapping_rain_water/solution.cpp
)

message(STATUS "Day 18 targets:")
message(STATUS "  day18_main        - 运行所有演示")
message(STATUS "  day18_call_stack  - 函数调用栈演示")
message(STATUS "  day18_stack_frame - 栈帧演示")
message(STATUS "  day18_enum_class  - enum class演示")
message(STATUS "  day18_item10      - EMC++ Item 10")
message(STATUS "  day18_lc84        - LeetCode 84")
message(STATUS "  day18_lc42        - LeetCode 42")
