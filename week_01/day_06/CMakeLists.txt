cmake_minimum_required(VERSION 3.10)
project(Day06_BinarySearch)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# 源文件
set(ALGORITHM_SOURCES
    code/algorithm/binary_search_basic.cpp
    code/algorithm/binary_search_left.cpp
    code/algorithm/binary_search_right.cpp
    code/algorithm/binary_search_template.cpp
)

set(LEETCODE_704_SOURCES
    code/leetcode/0704_binary_search/solution.cpp
)

set(LEETCODE_34_SOURCES
    code/leetcode/0034_find_first_and_last/solution.cpp
)

# 主程序
add_executable(binary_search_demo
    code/main.cpp
    ${LEETCODE_704_SOURCES}
    ${LEETCODE_34_SOURCES}
)

# 包含目录
target_include_directories(binary_search_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code/algorithm
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0704_binary_search
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0034_find_first_and_last
)

# LeetCode 704 测试程序
add_executable(test_0704
    code/leetcode/0704_binary_search/test.cpp
    code/leetcode/0704_binary_search/solution.cpp
)
target_include_directories(test_0704 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0704_binary_search
)

# LeetCode 34 测试程序
add_executable(test_0034
    code/leetcode/0034_find_first_and_last/test.cpp
    code/leetcode/0034_find_first_and_last/solution.cpp
)
target_include_directories(test_0034 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0034_find_first_and_last
)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
