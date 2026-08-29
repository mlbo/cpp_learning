cmake_minimum_required(VERSION 3.10)
project(day_02_arrays_decltype CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion
        -Wshadow -Werror
    )
endif()

# 源文件
set(SOURCES
    code/main.cpp
    code/data_structure/array_structure.cpp
    code/cpp11_features/decltype_demo.cpp
    code/emcpp/item06_auto_traps.cpp
    code/leetcode/0026_remove_duplicates/solution.cpp
    code/leetcode/0027_remove_element/solution.cpp
)

# 可执行文件
add_executable(day_02_demo ${SOURCES})

# 输出目录
set_target_properties(day_02_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 可失败的独立算法测试，避免只打印结果却始终返回成功。
add_executable(day_02_algorithm_tests
    code/leetcode/day02_algorithm_tests.cpp
    code/leetcode/0026_remove_duplicates/solution.cpp
    code/leetcode/0027_remove_element/solution.cpp
)

set_target_properties(day_02_algorithm_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

enable_testing()
add_test(NAME Day02ArrayAlgorithms COMMAND day_02_algorithm_tests)
add_test(NAME Day02MainSmoke COMMAND day_02_demo)

add_executable(day_02_remove_duplicates_tutorial_tests
    code/leetcode/0026_remove_duplicates/solution.cpp
    code/leetcode/0026_remove_duplicates/test.cpp
)
add_test(NAME Day02RemoveDuplicatesTutorialCases
         COMMAND day_02_remove_duplicates_tutorial_tests)

option(BUILD_EXTENDED_EXAMPLES "Build standalone Day 2 examples" ON)

if(BUILD_EXTENDED_EXAMPLES)
    add_executable(day_02_decltype_auto
        code/cpp11_features/decltype_auto.cpp
    )
    add_executable(day_02_decltype_rules
        code/cpp11_features/decltype_rules.cpp
    )
    add_executable(day_02_array_vs_vector
        code/data_structure/array_vs_vector.cpp
    )

    set_target_properties(
        day_02_decltype_auto
        day_02_decltype_rules
        day_02_array_vs_vector
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )

    add_test(NAME Day02DecltypeAutoSmoke COMMAND day_02_decltype_auto)
    add_test(NAME Day02DecltypeRulesSmoke COMMAND day_02_decltype_rules)
    add_test(NAME Day02ArrayVsVectorSmoke COMMAND day_02_array_vs_vector)
endif()
