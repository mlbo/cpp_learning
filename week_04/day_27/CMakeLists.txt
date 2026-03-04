cmake_minimum_required(VERSION 3.10)
project(day_27_string专题)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加编译选项
add_compile_options(-Wall -Wextra -g)

# 包含公共头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../common/include)

# 数据结构演示
add_executable(string_operations_demo
    code/data_structure/string_operations_demo.cpp
)

add_executable(sliding_window_demo
    code/data_structure/sliding_window_demo.cpp
)

add_executable(string_matching_demo
    code/data_structure/string_matching_demo.cpp
)

# LeetCode 题解（只编译test.cpp，因为test.cpp包含了solution.cpp）
add_executable(lc0076_minimum_window
    code/leetcode/0076_minimum_window/test.cpp
)

add_executable(lc0567_permutation_in_string
    code/leetcode/0567_permutation_in_string/test.cpp
)

# 主程序
add_executable(day_27_demo
    code/main.cpp
)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message(STATUS "Day 27 - 字符串专题")
message(STATUS "构建目标:")
message(STATUS "  - day_27_demo: 主程序演示")
message(STATUS "  - string_operations_demo: 字符串操作演示")
message(STATUS "  - sliding_window_demo: 滑动窗口演示")
message(STATUS "  - string_matching_demo: 字符串匹配演示")
message(STATUS "  - lc0076_minimum_window: LeetCode 76题解")
message(STATUS "  - lc0567_permutation_in_string: LeetCode 567题解")
