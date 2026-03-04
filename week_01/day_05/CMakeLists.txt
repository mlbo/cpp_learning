cmake_minimum_required(VERSION 3.16)
project(Day05_Constexpr_SlidingWindow CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项
add_compile_options(-Wall -Wextra -Wpedantic)

# 主程序
add_executable(day05_main
    code/main.cpp
)

# constexpr示例程序
add_executable(constexpr_var
    code/cpp11_features/constexpr_var.cpp
)

add_executable(constexpr_func
    code/cpp11_features/constexpr_func.cpp
)

add_executable(constexpr_ctor
    code/cpp11_features/constexpr_ctor.cpp
)

add_executable(constexpr_if
    code/cpp11_features/constexpr_if.cpp
)
target_compile_features(constexpr_if PRIVATE cxx_std_20)

# LeetCode 209题
add_executable(leetcode_0209
    code/leetcode/0209_minimum_size_subarray_sum/solution.cpp
    code/leetcode/0209_minimum_size_subarray_sum/test.cpp
)

# LeetCode 3题
add_executable(leetcode_0003
    code/leetcode/0003_longest_substring/solution.cpp
    code/leetcode/0003_longest_substring/test.cpp
)

# 添加测试输出信息
message(STATUS "=== Day 5 构建配置 ===")
message(STATUS "C++ 标准: ${CMAKE_CXX_STANDARD}")
message(STATUS "编译器: ${CMAKE_CXX_COMPILER}")
message(STATUS "目标程序:")
message(STATUS "  - day05_main: 主程序演示")
message(STATUS "  - constexpr_var: constexpr变量示例")
message(STATUS "  - constexpr_func: constexpr函数示例")
message(STATUS "  - constexpr_ctor: constexpr构造函数示例")
message(STATUS "  - constexpr_if: if constexpr示例")
message(STATUS "  - leetcode_0209: LeetCode 209题")
message(STATUS "  - leetcode_0003: LeetCode 3题")
