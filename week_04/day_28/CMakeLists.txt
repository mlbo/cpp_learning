# ========================================
# Day 28: 第四周复习 CMake配置
# ========================================

cmake_minimum_required(VERSION 3.14)
project(day_28_learning LANGUAGES CXX)

include(CTest)

# 本周统一使用严格 C++17，禁用编译器扩展模式。
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(WEEK04_SHARED_CMAKE
    "${CMAKE_CURRENT_LIST_DIR}/../cmake/Week04Sanitizers.cmake")
if(NOT EXISTS "${WEEK04_SHARED_CMAKE}")
    message(FATAL_ERROR
        "Week 4 的可搬迁边界是整棵 week_04：缺少共享 CMake helper。"
        "请复制完整 week_04，不要只复制 day_28。")
endif()
include("${WEEK04_SHARED_CMAKE}")
week04_require_complete_tree("${CMAKE_CURRENT_LIST_DIR}/..")
week04_enable_sanitizers()

# 编译选项
add_compile_options(-Wall -Wextra -Wpedantic)

# ========================================
# 数据结构复习
# ========================================

# 哈希表复习示例
add_library(day28_hash_table_review STATIC
    code/data_structure/hash_table_review.cpp
)
target_include_directories(day28_hash_table_review PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/code
)

add_executable(day28_hash_table_exception_tests
    code/tests/day28_hash_table_exception_tests.cpp
)
target_link_libraries(day28_hash_table_exception_tests PRIVATE
    day28_hash_table_review
)

# ========================================
# C++11特性复习
# ========================================

# 移动语义复习示例
add_library(day28_move_semantics_review STATIC
    code/cpp11_features/move_semantics_review.cpp
)

# ========================================
# EMC++条款复习
# ========================================

# Item 9, 23-30 复习示例
add_library(day28_emcpp_review STATIC
    code/emcpp/item09_23_30_review.cpp
)

add_executable(day28_semantic_contracts
    code/tests/day28_semantic_contracts.cpp
)
target_link_libraries(day28_semantic_contracts PRIVATE day28_emcpp_review)

# ========================================
# LeetCode题目
# ========================================

# LC 146: LRU缓存模块与测试使用者
add_library(day28_lru_cache STATIC
    code/leetcode/0146_lru_cache/solution.cpp
)
target_include_directories(day28_lru_cache PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0146_lru_cache
)

add_executable(day28_lc0146
    code/leetcode/0146_lru_cache/test.cpp
)
target_link_libraries(day28_lc0146 PRIVATE day28_lru_cache)

# LC 460: LFU缓存模块与测试使用者
add_library(day28_lfu_cache STATIC
    code/leetcode/0460_lfu_cache/solution.cpp
)
target_include_directories(day28_lfu_cache PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0460_lfu_cache
)

add_executable(day28_lc0460
    code/leetcode/0460_lfu_cache/test.cpp
)
target_link_libraries(day28_lc0460 PRIVATE day28_lfu_cache)

# Week 4 公开头边界：每个头在独立翻译单元中编译，再在一个翻译单元中组合。
set(WEEK04_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(WEEK04_PUBLIC_HEADERS
    day_22/code/cpp11_features/rvalue_reference_demo.h
    day_22/code/data_structure/hash_table_demo.h
    day_22/code/emcpp/item09_type_alias.h
    day_22/code/leetcode/0242_valid_anagram/solution.h
    day_22/code/leetcode/0383_ransom_note/solution.h
    day_24/code/leetcode/0049_group_anagrams/solution.h
    day_24/code/leetcode/0128_longest_consecutive/solution.h
    day_24/code/cpp11_features/universal_reference_demo.h
    day_25/code/cpp11_features/perfect_forwarding_demo.h
    day_25/code/leetcode/0003_longest_substring/solution.h
    day_25/code/leetcode/0438_find_anagrams/solution.h
    day_26/code/data_structure/cache_friendly_demo.h
    day_26/code/data_structure/cpu_cache_demo.h
    day_26/code/data_structure/memory_alignment_demo.h
    day_26/code/leetcode/0005_longest_palindrome/solution.h
    day_26/code/leetcode/0647_palindromic_substrings/solution.h
    day_27/code/leetcode/0076_minimum_window/solution.h
    day_27/code/leetcode/0567_permutation_in_string/solution.h
    day_28/code/leetcode/0146_lru_cache/solution.h
    day_28/code/leetcode/0460_lfu_cache/solution.h
    day_28/code/data_structure/simple_hash_table.h
)

set(HEADER_CHECK_DIR ${CMAKE_CURRENT_BINARY_DIR}/header_checks)
file(MAKE_DIRECTORY ${HEADER_CHECK_DIR})
foreach(header IN LISTS WEEK04_PUBLIC_HEADERS)
    string(MAKE_C_IDENTIFIER ${header} header_id)
    set(check_source ${HEADER_CHECK_DIR}/${header_id}.cpp)
    file(WRITE ${check_source} "#include \"${WEEK04_ROOT}/${header}\"\n")
    add_library(week04_header_${header_id} OBJECT ${check_source})
endforeach()

add_executable(day28_week04_public_headers
    code/tests/week04_public_headers.cpp
)
target_include_directories(day28_week04_public_headers PRIVATE
    ${WEEK04_ROOT}/day_22/code
    ${WEEK04_ROOT}/day_24/code
    ${WEEK04_ROOT}/day_25/code
    ${WEEK04_ROOT}/day_26/code
    ${WEEK04_ROOT}/day_27/code
    ${WEEK04_ROOT}/day_28/code
)

if(BUILD_TESTING)
    set(WEEK04_README_VERIFIER "${WEEK04_ROOT}/verify_readme_programs.sh")
    add_test(NAME day28_lru_cache COMMAND day28_lc0146)
    add_test(NAME day28_lfu_cache COMMAND day28_lc0460)
    add_test(NAME day28_hash_table_exception_contracts
        COMMAND day28_hash_table_exception_tests)
    add_test(NAME day28_week04_public_headers COMMAND day28_week04_public_headers)
    add_test(NAME day28_semantic_contracts COMMAND day28_semantic_contracts)
    add_test(NAME day28_readme_programs
        COMMAND ${CMAKE_COMMAND} -E env "CXX=${CMAKE_CXX_COMPILER}"
                bash ${WEEK04_README_VERIFIER} 28
    )
    set_tests_properties(day28_readme_programs PROPERTIES
        LABELS "readme;strict-no-sanitizer"
        TIMEOUT 120
    )
endif()

# ========================================
# 综合主程序
# ========================================

add_executable(day_28_main
    code/main.cpp
)
target_link_libraries(day_28_main PRIVATE
    day28_hash_table_review
    day28_move_semantics_review
    day28_emcpp_review
)

if(BUILD_TESTING)
    add_test(NAME day28_main_contract COMMAND day_28_main)
endif()

# ========================================
# 消息输出
# ========================================

message(STATUS "Day 28 项目配置完成")
message(STATUS "可执行文件:")
message(STATUS "  - day_28_main          : 综合主程序")
message(STATUS "  - day28_lc0146         : LRU缓存测试")
message(STATUS "  - day28_lc0460         : LFU缓存测试")
message(STATUS "  - day28_hash_table_exception_tests : rehash强异常保证测试")
message(STATUS "  - day28_week04_public_headers : Week 4公开头组合测试")
message(STATUS "  - day28_semantic_contracts : EMC++真实重载与失败边界契约")
message(STATUS "静态库（由演示或测试 target 链接，不可直接运行）:")
message(STATUS "  - day28_hash_table_review")
message(STATUS "  - day28_move_semantics_review")
message(STATUS "  - day28_emcpp_review")
message(STATUS "  - day28_lru_cache")
message(STATUS "  - day28_lfu_cache")
