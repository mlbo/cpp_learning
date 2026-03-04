cmake_minimum_required(VERSION 3.14)
project(day26_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# CPU缓存示例
add_library(day26_cpu_cache STATIC
    code/data_structure/cpu_cache_demo.cpp
)

# 内存对齐示例
add_library(day26_memory_alignment STATIC
    code/data_structure/memory_alignment_demo.cpp
)

# 缓存友好编程示例
add_library(day26_cache_friendly STATIC
    code/data_structure/cache_friendly_demo.cpp
)

# LeetCode 5 最长回文子串
add_library(day26_lc5 STATIC
    code/leetcode/0005_longest_palindrome/solution.cpp
)

# LeetCode 647 回文子串
add_library(day26_lc647 STATIC
    code/leetcode/0647_palindromic_substrings/solution.cpp
)

# 主程序（运行所有示例）
add_executable(day26_main
    code/main.cpp
    code/data_structure/cpu_cache_demo.cpp
    code/data_structure/memory_alignment_demo.cpp
    code/data_structure/cache_friendly_demo.cpp
    code/leetcode/0005_longest_palindrome/solution.cpp
    code/leetcode/0647_palindromic_substrings/solution.cpp
)

message(STATUS "Day 26 targets:")
message(STATUS "  day26_main             - 运行所有示例")
message(STATUS "  day26_cpu_cache        - CPU缓存演示")
message(STATUS "  day26_memory_alignment - 内存对齐演示")
message(STATUS "  day26_cache_friendly   - 缓存友好编程演示")
message(STATUS "  day26_lc5              - LeetCode 5 最长回文子串")
message(STATUS "  day26_lc647            - LeetCode 647 回文子串")
