cmake_minimum_required(VERSION 3.10)
project(Day06_BinarySearch)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 严格告警是教学契约的一部分，Release 和 Debug 都使用同一组诊断。
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion
        -Wshadow -Werror
    )
endif()

# 源文件
set(ALGORITHM_SOURCES
    code/algorithm/binary_search_basic.cpp
    code/algorithm/binary_search_left.cpp
    code/algorithm/binary_search_right.cpp
)

set(LEETCODE_704_SOURCES
    code/leetcode/0704_binary_search/solution.cpp
)

set(LEETCODE_34_SOURCES
    code/leetcode/0034_find_first_and_last/solution.cpp
)

# 需在创建目标前设置，否则已创建目标不会自动继承该默认值。
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 主程序
add_executable(binary_search_demo
    code/main.cpp
    ${ALGORITHM_SOURCES}
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

enable_testing()
add_test(NAME Day06BinarySearchDemo COMMAND binary_search_demo)
add_test(NAME Day06LeetCode704 COMMAND test_0704)
add_test(NAME Day06LeetCode34 COMMAND test_0034)
