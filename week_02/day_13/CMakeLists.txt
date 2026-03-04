cmake_minimum_required(VERSION 3.14)
project(day_13_list_practice)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
add_compile_options(-Wall -Wextra -Wpedantic)
find_package(Threads REQUIRED)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/code)
include_directories(${CMAKE_SOURCE_DIR}/code/leetcode/0160_intersection)
include_directories(${CMAKE_SOURCE_DIR}/code/leetcode/0148_sort_list)

# LeetCode 160: 相交链表
add_executable(test_160_intersection
    code/leetcode/0160_intersection/solution.cpp
    code/leetcode/0160_intersection/test.cpp
)
target_link_libraries(test_160_intersection PRIVATE Threads::Threads)

# LeetCode 148: 排序链表
add_executable(test_148_sort_list
    code/leetcode/0148_sort_list/solution.cpp
    code/leetcode/0148_sort_list/test.cpp
)
target_link_libraries(test_148_sort_list PRIVATE Threads::Threads)

# 主程序
add_executable(day_13
    code/main.cpp
    code/practice/list_summary.cpp
    code/practice/list_tricks.cpp
    code/practice/smart_ptr_summary.cpp
    code/leetcode/0160_intersection/solution.cpp
    code/leetcode/0148_sort_list/solution.cpp
)
target_link_libraries(day_13 PRIVATE Threads::Threads)

# 测试命令
enable_testing()
add_test(NAME IntersectionTest COMMAND test_160_intersection)
add_test(NAME SortListTest COMMAND test_148_sort_list)
