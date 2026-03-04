cmake_minimum_required(VERSION 3.14)
project(day_08_linked_list_unique_ptr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 编译选项
add_compile_options(-Wall -Wextra -Wpedantic)

# 添加调试信息
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/code)
include_directories(${CMAKE_SOURCE_DIR}/../../common/include)

# ============ 数据结构模块 ============
add_library(list_data_structure STATIC
    code/data_structure/list_operations.cpp
)

target_include_directories(list_data_structure PUBLIC
    ${CMAKE_SOURCE_DIR}/code/data_structure
)

# ============ C++11特性模块 ============
# unique_ptr基本演示
add_executable(unique_ptr_demo
    code/cpp11_features/unique_ptr_demo.cpp
)
target_link_libraries(unique_ptr_demo PRIVATE list_data_structure)

# unique_ptr高级用法
add_executable(unique_ptr_advanced
    code/cpp11_features/unique_ptr_advanced.cpp
)

# make_unique用法
add_executable(make_unique_demo
    code/cpp11_features/make_unique.cpp
)

# ============ EMC++条款模块 ============
# 条款17-18合并演示
add_executable(item17_item18
    code/emcpp/item17_item18.cpp
)

# 原有分离文件（保留）
add_executable(item17_special_members
    code/emcpp/item17_special_members.cpp
)

add_executable(item18_unique_ptr
    code/emcpp/item18_unique_ptr.cpp
)

# ============ LeetCode题目模块 ============
# LeetCode 203: 移除链表元素
add_executable(leetcode_0203
    code/leetcode/0203_remove_elements/solution.cpp
)
target_include_directories(leetcode_0203 PRIVATE
    ${CMAKE_SOURCE_DIR}/code/leetcode/0203_remove_elements
)
target_link_libraries(leetcode_0203 PRIVATE list_data_structure)

# LeetCode 206: 反转链表
add_executable(leetcode_0206
    code/leetcode/0206_reverse_list/solution.cpp
)
target_include_directories(leetcode_0206 PRIVATE
    ${CMAKE_SOURCE_DIR}/code/leetcode/0206_reverse_list
)
target_link_libraries(leetcode_0206 PRIVATE list_data_structure)

# ============ 主程序 ============
add_executable(day_08_demo
    code/main.cpp
)
target_link_libraries(day_08_demo PRIVATE list_data_structure)

# ============ 测试命令 ============
# 运行所有程序
add_custom_target(run_all
    COMMAND echo "========== 运行 unique_ptr_demo =========="
    COMMAND unique_ptr_demo
    COMMAND echo ""
    COMMAND echo "========== 运行 unique_ptr_advanced =========="
    COMMAND unique_ptr_advanced
    COMMAND echo ""
    COMMAND echo "========== 运行 make_unique_demo =========="
    COMMAND make_unique_demo
    COMMAND echo ""
    COMMAND echo "========== 运行 item17_item18 (合并版) =========="
    COMMAND item17_item18
    COMMAND echo ""
    COMMAND echo "========== 运行 LeetCode 203 =========="
    COMMAND leetcode_0203
    COMMAND echo ""
    COMMAND echo "========== 运行 LeetCode 206 =========="
    COMMAND leetcode_0206
    COMMAND echo ""
    COMMAND echo "========== 运行 day_08_demo =========="
    COMMAND day_08_demo
    DEPENDS unique_ptr_demo unique_ptr_advanced make_unique_demo
            item17_item18
            leetcode_0203 leetcode_0206 day_08_demo
)
