cmake_minimum_required(VERSION 3.14)
project(day25_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# 完美转发演示
add_library(day25_perfect_forward STATIC
    code/cpp11_features/perfect_forwarding_demo.cpp
)

# std::forward演示
add_library(day25_std_forward STATIC
    code/cpp11_features/std_forward_demo.cpp
)

# EMC++ Item 29
add_library(day25_item29 STATIC
    code/emcpp/item29_std_forward.cpp
)

# EMC++ Item 30
add_library(day25_item30 STATIC
    code/emcpp/item30_perfect_forward_fail.cpp
)

# LeetCode 3
add_library(day25_lc3 STATIC
    code/leetcode/0003_longest_substring/solution.cpp
)

# LeetCode 438
add_library(day25_lc438 STATIC
    code/leetcode/0438_find_anagrams/solution.cpp
)

# 主程序
add_executable(day25_main
    code/main.cpp
    code/cpp11_features/perfect_forwarding_demo.cpp
    code/cpp11_features/std_forward_demo.cpp
    code/emcpp/item29_std_forward.cpp
    code/emcpp/item30_perfect_forward_fail.cpp
    code/leetcode/0003_longest_substring/solution.cpp
    code/leetcode/0438_find_anagrams/solution.cpp
)

message(STATUS "Day 25 targets:")
message(STATUS "  day25_main            - 运行所有演示")
message(STATUS "  day25_perfect_forward - 完美转发演示")
message(STATUS "  day25_std_forward     - std::forward演示")
message(STATUS "  day25_item29          - EMC++ Item 29")
message(STATUS "  day25_item30          - EMC++ Item 30")
message(STATUS "  day25_lc3             - LeetCode 3")
message(STATUS "  day25_lc438           - LeetCode 438")
