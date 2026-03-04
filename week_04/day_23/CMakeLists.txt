cmake_minimum_required(VERSION 3.10)
project(day_23_move_semantics CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# 收集所有源文件
set(SOURCES
    code/main.cpp
    code/cpp11_features/move_semantics_demo.cpp
    code/cpp11_features/move_constructor_demo.cpp
    code/emcpp/item23_std_move.cpp
    code/emcpp/item24_universal_reference.cpp
    code/emcpp/item25_std_forward.cpp
    code/leetcode/0001_two_sum/solution.cpp
    code/leetcode/0454_4sum_ii/solution.cpp
)

# 创建可执行文件
add_executable(day_23_main ${SOURCES})

# 添加头文件目录
target_include_directories(day_23_main PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/code/cpp11_features
    ${CMAKE_CURRENT_SOURCE_DIR}/code/emcpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0001_two_sum
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0454_4sum_ii
)

# 输出编译信息
message(STATUS "Day 23: 移动语义")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
