cmake_minimum_required(VERSION 3.14)
project(day22_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)
set(WEEK04_SHARED_CMAKE
    "${CMAKE_CURRENT_LIST_DIR}/../cmake/Week04Sanitizers.cmake")
if(NOT EXISTS "${WEEK04_SHARED_CMAKE}")
    message(FATAL_ERROR
        "Week 4 的可搬迁边界是整棵 week_04：缺少共享 CMake helper。"
        "请复制完整 week_04，不要只复制 day_22。")
endif()
include("${WEEK04_SHARED_CMAKE}")
week04_require_complete_tree("${CMAKE_CURRENT_LIST_DIR}/..")
week04_enable_sanitizers()

# 哈希表演示
add_library(day22_hash_table STATIC
    code/data_structure/hash_table_demo.cpp
)

# 右值引用演示
add_library(day22_rvalue_reference STATIC
    code/cpp11_features/rvalue_reference_demo.cpp
)

# EMC++ Item 09 类型别名
add_library(day22_emcpp_item09 STATIC
    code/emcpp/item09_type_alias.cpp
)

# LeetCode 242 有效的字母异位词
add_library(day22_lc242 STATIC
    code/leetcode/0242_valid_anagram/solution.cpp
)

# LeetCode 383 赎金信
add_library(day22_lc383 STATIC
    code/leetcode/0383_ransom_note/solution.cpp
)

set(DAY22_MODULES
    day22_hash_table
    day22_rvalue_reference
    day22_emcpp_item09
    day22_lc242
    day22_lc383
)
foreach(module IN LISTS DAY22_MODULES)
    target_include_directories(${module} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/code
    )
endforeach()

# 主程序
add_executable(day22_main
    code/main.cpp
)
target_link_libraries(day22_main PRIVATE ${DAY22_MODULES})

add_executable(day22_contract_tests
    code/tests/day22_contract_tests.cpp
)
target_link_libraries(day22_contract_tests PRIVATE
    day22_hash_table
    day22_rvalue_reference
    day22_lc242
    day22_lc383
)

enable_testing()
set(WEEK04_README_VERIFIER
    "${CMAKE_CURRENT_SOURCE_DIR}/../verify_readme_programs.sh")
add_test(NAME Day22Contracts COMMAND day22_contract_tests)
add_test(NAME Day22Smoke COMMAND day22_main)
add_test(NAME Day22ReadmePrograms
    COMMAND ${CMAKE_COMMAND} -E env "CXX=${CMAKE_CXX_COMPILER}"
            bash ${WEEK04_README_VERIFIER} 22
)
set_tests_properties(Day22ReadmePrograms PROPERTIES
    LABELS "readme;strict-no-sanitizer"
    TIMEOUT 120
)

message(STATUS "Day 22 targets:")
message(STATUS "  day22_main            - 运行所有演示")
message(STATUS "  day22_hash_table      - 哈希表演示")
message(STATUS "  day22_rvalue_reference - 右值引用演示")
message(STATUS "  day22_emcpp_item09    - EMC++ Item 09")
message(STATUS "  day22_lc242           - LeetCode 242")
message(STATUS "  day22_lc383           - LeetCode 383")
message(STATUS "  day22_contract_tests  - 可失败契约测试")
