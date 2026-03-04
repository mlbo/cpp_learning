cmake_minimum_required(VERSION 3.14)
project(day14_week2_review CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
add_compile_options(-Wall -Wextra -Wpedantic)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()

# 线程支持
find_package(Threads REQUIRED)

# ==================== 库文件 ====================

# Review模块
add_library(review_lib STATIC
    code/review/week2_summary.cpp
    code/review/emcpp_review.cpp
    code/review/exercises.cpp
)
target_include_directories(review_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)

# Project模块 - 线程安全链表
add_library(thread_safe_list_lib STATIC
    code/project/thread_safe_list.cpp
)
target_include_directories(thread_safe_list_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
target_link_libraries(thread_safe_list_lib PUBLIC Threads::Threads)

# LeetCode 234 - 回文链表
add_library(leetcode234_lib STATIC
    code/leetcode/0234_palindrome/solution.cpp
)
target_include_directories(leetcode234_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)

# LeetCode 138 - 随机链表复制
add_library(leetcode138_lib STATIC
    code/leetcode/0138_copy_random/solution.cpp
)
target_include_directories(leetcode138_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)

# ==================== 可执行文件 ====================

# 主程序
add_executable(day14_main
    code/main.cpp
)
target_link_libraries(day14_main
    review_lib
    thread_safe_list_lib
    leetcode234_lib
    leetcode138_lib
    Threads::Threads
)

# 测试程序
add_executable(test_thread_safe_list
    code/project/test_thread_safe.cpp
)
target_link_libraries(test_thread_safe_list
    thread_safe_list_lib
    Threads::Threads
)

add_executable(test_leetcode234
    code/leetcode/0234_palindrome/test.cpp
)
target_link_libraries(test_leetcode234 leetcode234_lib)

add_executable(test_leetcode138
    code/leetcode/0138_copy_random/test.cpp
)
target_link_libraries(test_leetcode138 leetcode138_lib)

# ==================== 测试 ====================

enable_testing()

add_test(NAME TestThreadSafeList COMMAND test_thread_safe_list)
add_test(NAME TestLeetCode234 COMMAND test_leetcode234)
add_test(NAME TestLeetCode138 COMMAND test_leetcode138)

# ==================== 输出信息 ====================

message(STATUS "=== Day 14: 第二周复习与综合练习 ===")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "")
message(STATUS "可执行目标:")
message(STATUS "  - day14_main          : 主程序")
message(STATUS "  - test_thread_safe_list: 线程安全链表测试")
message(STATUS "  - test_leetcode234    : 回文链表测试")
message(STATUS "  - test_leetcode138    : 随机链表复制测试")
