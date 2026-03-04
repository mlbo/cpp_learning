cmake_minimum_required(VERSION 3.10)
project(Day04_Nullptr_And_TwoPointers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 定义源文件列表
set(CPP11_SOURCES
    code/cpp11_features/nullptr_demo.cpp
    code/cpp11_features/nullptr_vs_null.cpp
    code/cpp11_features/nullptr_overload.cpp
)

set(EMCPP_SOURCES
    code/emcpp/item08_prefer_nullptr.cpp
)

set(LEETCODE_SOURCES
    code/leetcode/0011_container_with_most_water/solution.cpp
    code/leetcode/0011_container_with_most_water/test.cpp
    code/leetcode/0015_3sum/solution.cpp
    code/leetcode/0015_3sum/test.cpp
)

# 主程序
add_executable(day04_main
    code/main.cpp
    ${CPP11_SOURCES}
    ${EMCPP_SOURCES}
    ${LEETCODE_SOURCES}
)

target_include_directories(day04_main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0011_container_with_most_water
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0015_3sum
)

# 添加测试目标
add_executable(day04_test_leetcode11
    code/leetcode/0011_container_with_most_water/solution.cpp
    code/leetcode/0011_container_with_most_water/test.cpp
)
target_include_directories(day04_test_leetcode11 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0011_container_with_most_water
)

add_executable(day04_test_leetcode15
    code/leetcode/0015_3sum/solution.cpp
    code/leetcode/0015_3sum/test.cpp
)
target_include_directories(day04_test_leetcode15 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0015_3sum
)

# 打印配置信息
message(STATUS "=== Day 4 Build Configuration ===")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
