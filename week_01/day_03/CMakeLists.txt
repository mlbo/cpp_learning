cmake_minimum_required(VERSION 3.14)
project(day03_tutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)
endif()

# 源文件收集
set(CPP11_SOURCES
    code/cpp11_features/uniform_init.cpp
    code/cpp11_features/initializer_list.cpp
    code/cpp11_features/init_comparison.cpp
)

set(EMCPP_SOURCES
    code/emcpp/item07_init_choice.cpp
)

set(LEETCODE_SOURCES
    code/leetcode/0088_merge_sorted_array/solution.cpp
    code/leetcode/0088_merge_sorted_array/test.cpp
    code/leetcode/0283_move_zeroes/solution.cpp
    code/leetcode/0283_move_zeroes/test.cpp
)

set(ALL_SOURCES
    code/main.cpp
    ${CPP11_SOURCES}
    ${EMCPP_SOURCES}
    ${LEETCODE_SOURCES}
)

# 主可执行文件
add_executable(day03_tutorial ${ALL_SOURCES})

target_include_directories(day03_tutorial PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0088_merge_sorted_array
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0283_move_zeroes
)

# 测试可执行文件（可选）
option(BUILD_TESTS "Build test executables" OFF)

if(BUILD_TESTS)
    # LeetCode 88 测试
    add_executable(test_0088
        code/leetcode/0088_merge_sorted_array/solution.cpp
        code/leetcode/0088_merge_sorted_array/test.cpp
    )
    target_include_directories(test_0088 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0088_merge_sorted_array
    )
    
    # LeetCode 283 测试
    add_executable(test_0283
        code/leetcode/0283_move_zeroes/solution.cpp
        code/leetcode/0283_move_zeroes/test.cpp
    )
    target_include_directories(test_0283 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0283_move_zeroes
    )
endif()

# 打印配置信息
message(STATUS "=== Day 3: C++11 统一初始化与初始化列表 ===")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
