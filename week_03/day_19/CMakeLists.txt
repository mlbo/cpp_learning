cmake_minimum_required(VERSION 3.14)
project(day19_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# 堆演示
add_executable(day19_heap
    code/data_structure/heap_demo.cpp
)

# 优先队列演示
add_executable(day19_pq
    code/data_structure/priority_queue_demo.cpp
)

# LeetCode 215
add_executable(day19_lc215
    code/leetcode/0215_kth_largest/solution.cpp
)

# LeetCode 347
add_executable(day19_lc347
    code/leetcode/0347_top_k_frequent/solution.cpp
)

# 主程序
add_executable(day19_main
    code/main.cpp
    code/data_structure/heap_demo.cpp
    code/data_structure/priority_queue_demo.cpp
    code/leetcode/0215_kth_largest/solution.cpp
    code/leetcode/0347_top_k_frequent/solution.cpp
)

message(STATUS "Day 19 targets:")
message(STATUS "  day19_main      - 运行所有演示")
message(STATUS "  day19_heap      - 堆演示")
message(STATUS "  day19_pq        - 优先队列演示")
message(STATUS "  day19_lc215     - LeetCode 215")
message(STATUS "  day19_lc347     - LeetCode 347")
