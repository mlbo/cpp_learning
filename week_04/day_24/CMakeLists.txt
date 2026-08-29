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
endif()

set(WEEK04_SHARED_CMAKE
    "${CMAKE_CURRENT_LIST_DIR}/../cmake/Week04Sanitizers.cmake")
if(NOT EXISTS "${WEEK04_SHARED_CMAKE}")
    message(FATAL_ERROR
        "Week 4 的可搬迁边界是整棵 week_04：缺少共享 CMake helper。"
        "请复制完整 week_04，不要只复制 day_24。")
endif()
include("${WEEK04_SHARED_CMAKE}")
week04_require_complete_tree("${CMAKE_CURRENT_LIST_DIR}/..")
week04_enable_sanitizers()

# 收集所有源文件
set(CPP11_SOURCES
    code/cpp11_features/universal_reference_demo.cpp
    code/cpp11_features/reference_collapsing_demo.cpp
)

set(EMCPP_SOURCES
    code/emcpp/item26_avoid_overloading.cpp
    code/emcpp/item28_folding_rules.cpp
    code/emcpp/item28_perfect_forward.cpp
)

set(LEETCODE_SOURCES
    code/leetcode/0049_group_anagrams/solution.cpp
    code/leetcode/0128_longest_consecutive/solution.cpp
)

# 课程实现先组成库，主程序与契约测试分别链接，避免测试复制源码清单。
add_library(day24_lessons STATIC
    ${CPP11_SOURCES}
    ${EMCPP_SOURCES}
    ${LEETCODE_SOURCES}
)

add_executable(day_24_demo code/main.cpp)
target_link_libraries(day_24_demo PRIVATE day24_lessons)

# 包含目录
target_include_directories(day24_lessons PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/code/cpp11_features
    ${CMAKE_CURRENT_SOURCE_DIR}/code/emcpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0049_group_anagrams
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0128_longest_consecutive
)

# 链接 pthread 库（如果需要）
find_package(Threads QUIET)
if(Threads_FOUND)
    target_link_libraries(day24_lessons PUBLIC Threads::Threads)
endif()

# 添加真实契约测试。默认开启，也可用 -DBUILD_TESTS=OFF 显式关闭。
option(BUILD_TESTS "Build test executables" ON)
if(BUILD_TESTS)
    enable_testing()
    set(WEEK04_README_VERIFIER
        "${CMAKE_CURRENT_SOURCE_DIR}/../verify_readme_programs.sh")

    add_executable(day24_contract_tests code/tests/day24_contract_tests.cpp)
    target_link_libraries(day24_contract_tests PRIVATE day24_lessons)
    add_test(NAME Day24Contracts COMMAND day24_contract_tests)
    add_test(NAME Day24Smoke COMMAND day_24_demo)
    add_test(NAME Day24ReadmePrograms
        COMMAND ${CMAKE_COMMAND} -E env "CXX=${CMAKE_CXX_COMPILER}"
                bash ${WEEK04_README_VERIFIER} 24
    )
    set_tests_properties(Day24ReadmePrograms PROPERTIES
        LABELS "readme;strict-no-sanitizer"
        TIMEOUT 120
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
