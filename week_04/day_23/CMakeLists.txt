cmake_minimum_required(VERSION 3.14)
project(day_23_move_semantics CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
add_compile_options(-Wall -Wextra -Wpedantic)
set(WEEK04_SHARED_CMAKE
    "${CMAKE_CURRENT_LIST_DIR}/../cmake/Week04Sanitizers.cmake")
if(NOT EXISTS "${WEEK04_SHARED_CMAKE}")
    message(FATAL_ERROR
        "Week 4 的可搬迁边界是整棵 week_04：缺少共享 CMake helper。"
        "请复制完整 week_04，不要只复制 day_23。")
endif()
include("${WEEK04_SHARED_CMAKE}")
week04_require_complete_tree("${CMAKE_CURRENT_LIST_DIR}/..")
week04_enable_sanitizers()

# 收集所有源文件
set(LESSON_SOURCES
    code/cpp11_features/move_semantics_demo.cpp
    code/cpp11_features/move_constructor_demo.cpp
    code/emcpp/item23_std_move.cpp
    code/emcpp/item24_universal_reference.cpp
    code/emcpp/item25_std_forward.cpp
    code/leetcode/0001_two_sum/solution.cpp
    code/leetcode/0454_4sum_ii/solution.cpp
)

add_library(day23_lessons STATIC ${LESSON_SOURCES})
add_executable(day_23_main code/main.cpp)
target_link_libraries(day_23_main PRIVATE day23_lessons)

# 添加头文件目录
target_include_directories(day23_lessons PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/code/cpp11_features
    ${CMAKE_CURRENT_SOURCE_DIR}/code/emcpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0001_two_sum
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0454_4sum_ii
)

add_executable(day23_contract_tests code/tests/day23_contract_tests.cpp)
target_link_libraries(day23_contract_tests PRIVATE day23_lessons)

option(BUILD_UNSAFE_LIFETIME_DEMO
    "Build the intentionally dangling-reference ASan teaching probe" OFF)
if(BUILD_UNSAFE_LIFETIME_DEMO)
    add_executable(day23_lifetime_boundary_asan_demo
        code/tests/lifetime_boundary_asan_demo.cpp
    )
endif()

enable_testing()
set(WEEK04_README_VERIFIER
    "${CMAKE_CURRENT_SOURCE_DIR}/../verify_readme_programs.sh")
add_test(NAME Day23Contracts COMMAND day23_contract_tests)
add_test(NAME Day23Smoke COMMAND day_23_main --all)
add_test(NAME Day23Help COMMAND day_23_main --help)
add_test(NAME Day23ReadmePrograms
    COMMAND ${CMAKE_COMMAND} -E env "CXX=${CMAKE_CXX_COMPILER}"
            bash ${WEEK04_README_VERIFIER} 23
)
set_tests_properties(Day23Smoke PROPERTIES TIMEOUT 10)
set_tests_properties(Day23Help PROPERTIES
    PASS_REGULAR_EXPRESSION "day_23_main.*\\[选项\\]"
)
set_tests_properties(Day23ReadmePrograms PROPERTIES
    LABELS "readme;strict-no-sanitizer"
    TIMEOUT 120
)

# 输出编译信息
message(STATUS "Day 23: 移动语义")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
