cmake_minimum_required(VERSION 3.14)
project(day25_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
option(BUILD_TESTS "Build Day 25 tests" ${BUILD_TESTING})
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
set(WEEK04_SHARED_CMAKE
    "${CMAKE_CURRENT_LIST_DIR}/../cmake/Week04Sanitizers.cmake")
if(NOT EXISTS "${WEEK04_SHARED_CMAKE}")
    message(FATAL_ERROR
        "Week 4 的可搬迁边界是整棵 week_04：缺少共享 CMake helper。"
        "请复制完整 week_04，不要只复制 day_25。")
endif()
include("${WEEK04_SHARED_CMAKE}")
week04_require_complete_tree("${CMAKE_CURRENT_LIST_DIR}/..")
week04_enable_sanitizers()

# 完美转发演示
add_library(day25_perfect_forward STATIC
    code/cpp11_features/perfect_forwarding_demo.cpp
)

# std::forward演示
add_library(day25_std_forward STATIC
    code/cpp11_features/std_forward_demo.cpp
)

# EMC++ Item 29（文件中同时保留 std::forward 前置知识回顾）
add_library(day25_item29 STATIC
    code/emcpp/item29_move_assumptions.cpp
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

set(DAY25_MODULES
    day25_perfect_forward
    day25_std_forward
    day25_item29
    day25_item30
    day25_lc3
    day25_lc438
)
foreach(module IN LISTS DAY25_MODULES)
    target_include_directories(${module} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/code
    )
endforeach()

# 主程序
add_executable(day25_main
    code/main.cpp
)
target_link_libraries(day25_main PRIVATE ${DAY25_MODULES})

if(BUILD_TESTS)
    enable_testing()
    set(WEEK04_README_VERIFIER
        "${CMAKE_CURRENT_SOURCE_DIR}/../verify_readme_programs.sh")
    add_executable(day25_tests
        code/tests/day25_tests.cpp
    )
    target_link_libraries(day25_tests PRIVATE
        day25_perfect_forward
        day25_std_forward
        day25_item29
        day25_item30
        day25_lc3
        day25_lc438
    )
    add_test(NAME Day25UnitTests COMMAND day25_tests)
    add_test(NAME Day25DemoSmoke COMMAND day25_main)
    add_test(NAME Day25ReadmePrograms
        COMMAND ${CMAKE_COMMAND} -E env "CXX=${CMAKE_CXX_COMPILER}"
                bash ${WEEK04_README_VERIFIER} 25
    )
    set_tests_properties(Day25DemoSmoke PROPERTIES TIMEOUT 30)
    set_tests_properties(Day25ReadmePrograms PROPERTIES
        LABELS "readme;strict-no-sanitizer"
        TIMEOUT 120
    )
endif()

message(STATUS "Day 25 targets:")
message(STATUS "  day25_main            - 运行所有演示")
message(STATUS "  day25_perfect_forward - 完美转发演示")
message(STATUS "  day25_std_forward     - std::forward演示")
message(STATUS "  day25_item29          - EMC++ Item 29 与前置知识回顾")
message(STATUS "  day25_item30          - EMC++ Item 30")
message(STATUS "  day25_lc3             - LeetCode 3")
message(STATUS "  day25_lc438           - LeetCode 438")
if(BUILD_TESTS)
    message(STATUS "  day25_tests           - 可失败的算法与转发语义测试")
endif()
