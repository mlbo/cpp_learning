cmake_minimum_required(VERSION 3.14)
project(day26_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
option(BUILD_TESTS "Build Day 26 tests" ${BUILD_TESTING})
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
set(WEEK04_SHARED_CMAKE
    "${CMAKE_CURRENT_LIST_DIR}/../cmake/Week04Sanitizers.cmake")
if(NOT EXISTS "${WEEK04_SHARED_CMAKE}")
    message(FATAL_ERROR
        "Week 4 的可搬迁边界是整棵 week_04：缺少共享 CMake helper。"
        "请复制完整 week_04，不要只复制 day_26。")
endif()
include("${WEEK04_SHARED_CMAKE}")
week04_require_complete_tree("${CMAKE_CURRENT_LIST_DIR}/..")
week04_enable_sanitizers()
find_package(Threads REQUIRED)

# CPU缓存示例
add_library(day26_cpu_cache STATIC
    code/data_structure/cpu_cache_demo.cpp
)
target_link_libraries(day26_cpu_cache PUBLIC Threads::Threads)

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

set(DAY26_MODULES
    day26_cpu_cache
    day26_memory_alignment
    day26_cache_friendly
    day26_lc5
    day26_lc647
)
foreach(module IN LISTS DAY26_MODULES)
    target_include_directories(${module} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/code
    )
endforeach()

# 主程序（运行所有示例）
add_executable(day26_main
    code/main.cpp
)
target_link_libraries(day26_main PRIVATE ${DAY26_MODULES})

if(BUILD_TESTS)
    enable_testing()
    add_executable(day26_tests
        code/tests/day26_tests.cpp
    )
    target_link_libraries(day26_tests PRIVATE
        day26_memory_alignment
        day26_lc5
        day26_lc647
    )
    add_test(NAME Day26UnitTests COMMAND day26_tests)
    add_test(NAME Day26Smoke COMMAND day26_main)
    set(WEEK04_README_VERIFIER
        "${CMAKE_CURRENT_SOURCE_DIR}/../verify_readme_programs.sh")
    add_test(NAME Day26ReadmePrograms
        COMMAND ${CMAKE_COMMAND} -E env "CXX=${CMAKE_CXX_COMPILER}"
                bash ${WEEK04_README_VERIFIER} 26
    )
    set_tests_properties(Day26ReadmePrograms PROPERTIES
        LABELS "readme;strict-no-sanitizer"
        TIMEOUT 120
    )
endif()

message(STATUS "Day 26 targets:")
message(STATUS "  day26_main             - 运行所有示例")
message(STATUS "  day26_cpu_cache        - CPU缓存演示")
message(STATUS "  day26_memory_alignment - 内存对齐演示")
message(STATUS "  day26_cache_friendly   - 缓存友好编程演示")
message(STATUS "  day26_lc5              - LeetCode 5 最长回文子串")
message(STATUS "  day26_lc647            - LeetCode 647 回文子串")
if(BUILD_TESTS)
    message(STATUS "  day26_tests            - 算法与安全对齐可失败测试")
endif()
