# Day 24: 通用引用 (Universal Reference)
# CMakeLists.txt - 项目构建配置

cmake_minimum_required(VERSION 3.14)

project(day_24_universal_reference 
    VERSION 1.0.0
    DESCRIPTION "Day 24: Universal Reference and Reference Collapsing Tutorial"
    LANGUAGES CXX
)

# 设置 C++ 标准和编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加编译警告
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror=return-type)
    # 添加调试信息
    add_compile_options(-g -O0)
endif()

# 公共头文件目录
set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../common/include)

# 收集所有源文件
set(CPP11_SOURCES
    code/cpp11_features/universal_reference_demo.cpp
    code/cpp11_features/reference_collapsing_demo.cpp
)

set(EMCPP_SOURCES
    code/emcpp/item26_avoid_overloading.cpp
    code/emcpp/item27_folding_rules.cpp
    code/emcpp/item28_perfect_forward.cpp
)

set(LEETCODE_SOURCES
    code/leetcode/0049_group_anagrams/solution.cpp
    code/leetcode/0128_longest_consecutive/solution.cpp
)

# 主可执行文件
add_executable(day_24_demo
    code/main.cpp
    ${CPP11_SOURCES}
    ${EMCPP_SOURCES}
    ${LEETCODE_SOURCES}
)

# 包含目录
target_include_directories(day_24_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/code/cpp11_features
    ${CMAKE_CURRENT_SOURCE_DIR}/code/emcpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0049_group_anagrams
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0128_longest_consecutive
    ${COMMON_INCLUDE_DIR}
)

# 链接 pthread 库（如果需要）
find_package(Threads QUIET)
if(Threads_FOUND)
    target_link_libraries(day_24_demo PRIVATE Threads::Threads)
endif()

# 添加测试目标（可选）
option(BUILD_TESTS "Build test executables" OFF)
if(BUILD_TESTS)
    enable_testing()
    
    # LeetCode 测试
    add_executable(test_0049
        code/leetcode/0049_group_anagrams/solution.cpp
        code/leetcode/0049_group_anagrams/test.cpp
    )
    target_include_directories(test_0049 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0049_group_anagrams
    )
    
    add_executable(test_0128
        code/leetcode/0128_longest_consecutive/solution.cpp
        code/leetcode/0128_longest_consecutive/test.cpp
    )
    target_include_directories(test_0128 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0128_longest_consecutive
    )
endif()

# 打印配置信息
message(STATUS "========================================")
message(STATUS "Day 24: 通用引用 (Universal Reference)")
message(STATUS "========================================")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "========================================")
