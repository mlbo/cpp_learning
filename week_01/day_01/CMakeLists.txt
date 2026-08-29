cmake_minimum_required(VERSION 3.10)
project(day_01_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion
        -Wshadow -Werror
    )
endif()

# 数据结构示例
add_library(day01_complexity STATIC
    code/data_structure/complexity.cpp
)

# C++11特性示例
add_library(day01_auto_demo STATIC
    code/cpp11_features/auto_demo.cpp
)

# EMC++条款示例
add_library(day01_emcpp STATIC
    code/emcpp/type_deduction_items.cpp
)

# LeetCode题目
add_library(day01_lc0001 STATIC
    code/leetcode/0001_two_sum/solution.cpp
    code/leetcode/0001_two_sum/test.cpp
)

add_library(day01_lc0167 STATIC
    code/leetcode/0167_two_sum_ii/solution.cpp
    code/leetcode/0167_two_sum_ii/test.cpp
)

# 主程序只编译入口自身；课程实现由上面的静态库提供，避免同一实现既建库又
# 被主入口重复编译。这个依赖关系也让“声明、编译、归档、链接”边界可见。
add_executable(day01_main
    code/main.cpp
)
target_link_libraries(day01_main PRIVATE
    day01_complexity
    day01_auto_demo
    day01_emcpp
    day01_lc0001
    day01_lc0167
)

# 独立算法测试使用真实题解实现，并在任何断言失败时返回非零状态。
add_executable(day01_algorithm_tests
    code/leetcode/day01_algorithm_tests.cpp
    code/leetcode/0001_two_sum/solution.cpp
    code/leetcode/0167_two_sum_ii/solution.cpp
)

add_executable(day01_lifetime_boundaries
    code/emcpp/lifetime_boundary_test.cpp
)
target_include_directories(day01_lifetime_boundaries PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code/emcpp
)

add_executable(day01_complexity_contracts
    code/data_structure/complexity_contract_test.cpp
    code/data_structure/complexity.cpp
)

# 早期独立实现仍作为对照教材，但必须真正编译并以退出码报告失败。
add_executable(day01_legacy_crosscheck
    code/test_all.cpp
)

set_target_properties(day01_algorithm_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 扩展示例也属于教程内容，默认参与构建，避免未被主程序引用的示例长期失效。
option(BUILD_EXTENDED_EXAMPLES "Build standalone Day 1 examples" ON)

if(BUILD_EXTENDED_EXAMPLES)
    add_executable(day01_auto_rules code/cpp11_features/auto_rules.cpp)
    add_executable(day01_item01 code/emcpp/item01_template_deduction.cpp)
    add_executable(day01_item02 code/emcpp/item02_auto_deduction.cpp)
    add_executable(day01_item03 code/emcpp/item03_decltype.cpp)
    add_executable(day01_item04 code/emcpp/item04_see_types.cpp)
    add_executable(day01_item05 code/emcpp/item05_prefer_auto.cpp)
endif()

enable_testing()
add_test(NAME Day01Algorithms COMMAND day01_algorithm_tests)
add_test(NAME Day01MainSmoke COMMAND day01_main)
add_test(NAME Day01LifetimeBoundaries COMMAND day01_lifetime_boundaries)
add_test(NAME Day01ComplexityContracts COMMAND day01_complexity_contracts)
add_test(NAME Day01LegacyCrosscheck COMMAND day01_legacy_crosscheck)

if(BUILD_EXTENDED_EXAMPLES)
    add_test(NAME Day01AutoRulesSmoke COMMAND day01_auto_rules)
    add_test(NAME Day01Item01Smoke COMMAND day01_item01)
    add_test(NAME Day01Item02Smoke COMMAND day01_item02)
    add_test(NAME Day01Item03Smoke COMMAND day01_item03)
    add_test(NAME Day01Item04Smoke COMMAND day01_item04)
    add_test(NAME Day01Item05Smoke COMMAND day01_item05)
endif()
