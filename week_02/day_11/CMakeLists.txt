cmake_minimum_required(VERSION 3.14)
project(cpp_day_11 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
)

# Debug模式
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0)
else()
    add_compile_options(-O2)
endif()

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/code)

# 主程序
add_executable(day_11_main
    code/main.cpp
    code/cpp11_features/pimpl_widget.cpp
    code/cpp11_features/pimpl_demo.cpp
)

# EMC++ 条款22示例
add_executable(item22_pimpl
    code/emcpp/item22_pimpl.cpp
)

# LeetCode 23: 合并K个升序链表
add_executable(leetcode_23
    code/leetcode/0023_merge_k_lists/solution.cpp
    code/leetcode/0023_merge_k_lists/test.cpp
)

# LeetCode 61: 旋转链表
add_executable(leetcode_61
    code/leetcode/0061_rotate_list/solution.cpp
    code/leetcode/0061_rotate_list/test.cpp
)

# 链接线程库（如果需要）
find_package(Threads REQUIRED)
target_link_libraries(day_11_main Threads::Threads)

# 测试目标
enable_testing()

add_test(NAME TestLeetCode23 COMMAND leetcode_23)
add_test(NAME TestLeetCode61 COMMAND leetcode_61)
