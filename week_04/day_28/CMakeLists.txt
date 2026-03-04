# ========================================
# Day 28: 第四周复习 CMake配置
# ========================================

cmake_minimum_required(VERSION 3.14)
project(day_28_learning LANGUAGES CXX)

# C++11 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# 公共头文件
include_directories(${CMAKE_SOURCE_DIR}/common/include)

# ========================================
# 数据结构复习
# ========================================

# 哈希表复习示例
add_executable(day28_hash_table_review
    code/data_structure/hash_table_review.cpp
)

# ========================================
# C++11特性复习
# ========================================

# 移动语义复习示例
add_executable(day28_move_semantics_review
    code/cpp11_features/move_semantics_review.cpp
)

# ========================================
# EMC++条款复习
# ========================================

# Item 9, 23-30 复习示例
add_executable(day28_emcpp_review
    code/emcpp/item09_23_30_review.cpp
)

# ========================================
# LeetCode题目
# ========================================

# LC 146: LRU缓存
add_executable(day28_lc0146
    code/leetcode/0146_lru_cache/solution.cpp
    code/leetcode/0146_lru_cache/test.cpp
)

# LC 460: LFU缓存
add_executable(day28_lc0460
    code/leetcode/0460_lfu_cache/solution.cpp
    code/leetcode/0460_lfu_cache/test.cpp
)

# ========================================
# 综合主程序
# ========================================

add_executable(day_28_main
    code/main.cpp
    code/data_structure/hash_table_review.cpp
    code/cpp11_features/move_semantics_review.cpp
    code/emcpp/item09_23_30_review.cpp
    code/leetcode/0146_lru_cache/solution.cpp
    code/leetcode/0146_lru_cache/test.cpp
    code/leetcode/0460_lfu_cache/solution.cpp
    code/leetcode/0460_lfu_cache/test.cpp
)

# ========================================
# 消息输出
# ========================================

message(STATUS "Day 28 项目配置完成")
message(STATUS "可执行文件:")
message(STATUS "  - day_28_main          : 综合主程序")
message(STATUS "  - day28_hash_table_review : 哈希表复习")
message(STATUS "  - day28_move_semantics_review : 移动语义复习")
message(STATUS "  - day28_emcpp_review   : EMC++条款复习")
message(STATUS "  - day28_lc0146         : LRU缓存测试")
message(STATUS "  - day28_lc0460         : LFU缓存测试")
