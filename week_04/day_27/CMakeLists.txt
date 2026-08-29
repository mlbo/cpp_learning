cmake_minimum_required(VERSION 3.14)
project(day_27_string专题)

include(CTest)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加编译选项
add_compile_options(-Wall -Wextra -Wpedantic)
set(WEEK04_SHARED_CMAKE
    "${CMAKE_CURRENT_LIST_DIR}/../cmake/Week04Sanitizers.cmake")
if(NOT EXISTS "${WEEK04_SHARED_CMAKE}")
    message(FATAL_ERROR
        "Week 4 的可搬迁边界是整棵 week_04：缺少共享 CMake helper。"
        "请复制完整 week_04，不要只复制 day_27。")
endif()
include("${WEEK04_SHARED_CMAKE}")
week04_require_complete_tree("${CMAKE_CURRENT_LIST_DIR}/..")
week04_enable_sanitizers()

# 数据结构演示
add_executable(string_operations_demo
    code/data_structure/string_operations_demo.cpp
)

add_executable(sliding_window_demo
    code/data_structure/sliding_window_demo.cpp
)

add_executable(string_matching_demo
    code/data_structure/string_matching_demo.cpp
)

# LeetCode 题解：实现各自编译成库，测试只包含公开头文件并链接库。
add_library(day27_lc0076 STATIC
    code/leetcode/0076_minimum_window/solution.cpp
)
target_include_directories(day27_lc0076 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/code
)

add_executable(lc0076_minimum_window
    code/leetcode/0076_minimum_window/test.cpp
)
target_link_libraries(lc0076_minimum_window PRIVATE day27_lc0076)

add_library(day27_lc0567 STATIC
    code/leetcode/0567_permutation_in_string/solution.cpp
)
target_include_directories(day27_lc0567 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/code
)

add_executable(lc0567_permutation_in_string
    code/leetcode/0567_permutation_in_string/test.cpp
)
target_link_libraries(lc0567_permutation_in_string PRIVATE day27_lc0567)

# 真实项目组合边界：一个翻译单元同时包含两道题的公开头并链接两个实现库。
add_executable(day27_solution_composition
    code/tests/day27_solution_composition.cpp
)
target_link_libraries(day27_solution_composition PRIVATE
    day27_lc0076
    day27_lc0567
)

if(BUILD_TESTING)
    add_test(NAME day27_lc0076_minimum_window COMMAND lc0076_minimum_window)
    add_test(NAME day27_lc0567_permutation_in_string COMMAND lc0567_permutation_in_string)
    add_test(NAME day27_solution_composition COMMAND day27_solution_composition)
    add_test(NAME day27_sliding_window_contract COMMAND sliding_window_demo)
    set(WEEK04_README_VERIFIER
        "${CMAKE_CURRENT_SOURCE_DIR}/../verify_readme_programs.sh")
    add_test(NAME day27_readme_programs
        COMMAND ${CMAKE_COMMAND} -E env "CXX=${CMAKE_CXX_COMPILER}"
                bash ${WEEK04_README_VERIFIER} 27
    )
    set_tests_properties(day27_readme_programs PROPERTIES
        LABELS "readme;strict-no-sanitizer"
        TIMEOUT 120
    )
endif()

# 主程序
add_executable(day_27_demo
    code/main.cpp
)

# 保持 CMake 默认的构建根目录输出，和 build_and_run.sh 的执行路径一致。

message(STATUS "Day 27 - 字符串专题")
message(STATUS "构建目标:")
message(STATUS "  - day_27_demo: 主程序演示")
message(STATUS "  - string_operations_demo: 字符串操作演示")
message(STATUS "  - sliding_window_demo: 滑动窗口演示")
message(STATUS "  - string_matching_demo: 字符串匹配演示")
message(STATUS "  - lc0076_minimum_window: LeetCode 76题解")
message(STATUS "  - lc0567_permutation_in_string: LeetCode 567题解")
message(STATUS "  - day27_solution_composition: 两个公开头的组合边界测试")
