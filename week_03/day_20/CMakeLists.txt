cmake_minimum_required(VERSION 3.14)
project(day20_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# BFS模板演示
add_executable(day20_bfs_template
    code/algorithm/bfs_template.cpp
)

# BFS演示
add_executable(day20_bfs_demo
    code/algorithm/bfs_demo.cpp
)

# LeetCode 102
add_executable(day20_lc102
    code/leetcode/0102_level_order/solution.cpp
)

# LeetCode 107
add_executable(day20_lc107
    code/leetcode/0107_level_order_bottom/solution.cpp
)

# 主程序
add_executable(day20_main
    code/main.cpp
    code/algorithm/bfs_template.cpp
    code/algorithm/bfs_demo.cpp
    code/data_structure/tree_node.cpp
    code/leetcode/0102_level_order/solution.cpp
    code/leetcode/0107_level_order_bottom/solution.cpp
)

message(STATUS "Day 20 targets:")
message(STATUS "  day20_main        - 运行所有演示")
message(STATUS "  day20_bfs_template - BFS模板演示")
message(STATUS "  day20_bfs_demo    - BFS演示")
message(STATUS "  day20_lc102       - LeetCode 102")
message(STATUS "  day20_lc107       - LeetCode 107")
