cmake_minimum_required(VERSION 3.14)
project(day22_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# 哈希表演示
add_executable(day22_hash_table
    code/data_structure/hash_table_demo.cpp
)

# 右值引用演示
add_executable(day22_rvalue_reference
    code/cpp11_features/rvalue_reference_demo.cpp
)

# EMC++ Item 09 类型别名
add_executable(day22_emcpp_item09
    code/emcpp/item09_type_alias.cpp
)

# LeetCode 242 有效的字母异位词
add_executable(day22_lc242
    code/leetcode/0242_valid_anagram/solution.cpp
)

# LeetCode 383 赎金信
add_executable(day22_lc383
    code/leetcode/0383_ransom_note/solution.cpp
)

# 主程序
add_executable(day22_main
    code/main.cpp
    code/data_structure/hash_table_demo.cpp
    code/cpp11_features/rvalue_reference_demo.cpp
    code/emcpp/item09_type_alias.cpp
    code/leetcode/0242_valid_anagram/solution.cpp
    code/leetcode/0383_ransom_note/solution.cpp
)

message(STATUS "Day 22 targets:")
message(STATUS "  day22_main            - 运行所有演示")
message(STATUS "  day22_hash_table      - 哈希表演示")
message(STATUS "  day22_rvalue_reference - 右值引用演示")
message(STATUS "  day22_emcpp_item09    - EMC++ Item 09")
message(STATUS "  day22_lc242           - LeetCode 242")
message(STATUS "  day22_lc383           - LeetCode 383")
