cmake_minimum_required(VERSION 3.14)
project(day_35_summary LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# 数据结构总结
add_executable(day35_knowledge_summary
    code/data_structure/knowledge_summary.cpp
)

# C++11特性综合
add_executable(day35_cpp11_summary
    code/cpp11_features/cpp11_summary.cpp
)
target_link_libraries(day35_cpp11_summary PRIVATE Threads::Threads)

# EMC++复习
add_executable(day35_emcpp_review
    code/emcpp/emcpp_review.cpp
)

# LeetCode题目
add_executable(day35_lc0297
    code/leetcode/0297_serialize_deserialize/solution.cpp
)

add_executable(day35_lc0124
    code/leetcode/0124_max_path_sum/solution.cpp
)

# 综合主程序
add_executable(day35_main
    code/main.cpp
    code/data_structure/knowledge_summary.cpp
    code/cpp11_features/cpp11_summary.cpp
    code/emcpp/emcpp_review.cpp
    code/leetcode/0297_serialize_deserialize/solution.cpp
    code/leetcode/0124_max_path_sum/solution.cpp
)
target_link_libraries(day35_main PRIVATE Threads::Threads)
