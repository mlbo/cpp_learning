cmake_minimum_required(VERSION 3.10)
project(day_10_weak_ptr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
find_package(Threads REQUIRED)

# 编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow
    )
endif()

# 可执行文件
add_executable(day_10_demo
    code/main.cpp
    code/cpp11_features/weak_ptr_demo.cpp
    code/emcpp/item21.cpp
    code/leetcode/0142_cycle_entrance/solution.cpp
    code/leetcode/0019_remove_nth/solution.cpp
)

target_link_libraries(day_10_demo PRIVATE Threads::Threads)

# 循环引用与观察者模式扩展示例
add_executable(day10_ownership_examples
    code/ownership_examples_main.cpp
    code/cpp11_features/break_cycle.cpp
    code/cpp11_features/observer_pattern.cpp
)

# 历史文件名 item21_weak_ptr.cpp，内容按正确编号归入 EMC++ Item 20。
add_executable(day10_item20_extended
    code/emcpp/item20_main.cpp
    code/emcpp/item21_weak_ptr.cpp
)
target_link_libraries(day10_item20_extended PRIVATE Threads::Threads)

# LeetCode 142：实现与测试分别编译，测试错误时返回非零。
add_executable(leetcode_0142_tests
    code/leetcode/0142_cycle_entrance/solution.cpp
    code/leetcode/0142_cycle_entrance/test.cpp
)
target_include_directories(leetcode_0142_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0142_cycle_entrance
)

# LeetCode 19：包含合法输入和非法 n 的边界测试。
add_executable(leetcode_0019_tests
    code/leetcode/0019_remove_nth/solution.cpp
    code/leetcode/0019_remove_nth/test_main.cpp
)

if(BUILD_TESTING)
    add_test(NAME day10_cycle_entrance COMMAND leetcode_0142_tests)
    add_test(NAME day10_remove_nth COMMAND leetcode_0019_tests)
    add_test(NAME day10_ownership_examples_smoke COMMAND day10_ownership_examples)
    add_test(NAME day10_item20_extended_smoke COMMAND day10_item20_extended)
    add_test(NAME day10_weak_ptr_smoke COMMAND day_10_demo)
endif()

add_custom_target(run_all
    COMMAND day_10_demo
    COMMAND day10_ownership_examples
    COMMAND day10_item20_extended
    DEPENDS day_10_demo day10_ownership_examples day10_item20_extended
)

# 设置输出目录
set_target_properties(day_10_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
