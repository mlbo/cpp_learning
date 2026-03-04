cmake_minimum_required(VERSION 3.14)
project(cpp_day_07 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
)

# Debug模式
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0)
else()
    add_compile_options(-O2)
endif()

# 主程序
add_executable(day_07_main
    code/main.cpp
    code/review/type_deduction_review.cpp
    code/review/init_review.cpp
    code/review/pointer_review.cpp
    code/project/dynamic_array.cpp
)

# LeetCode 42: 接雨水
add_executable(leetcode_42
    code/leetcode/0042_trapping_rain_water/test.cpp
)

# LeetCode 189: 轮转数组
add_executable(leetcode_189
    code/leetcode/0189_rotate_array/test.cpp
)

# 链接线程库（如果需要）
find_package(Threads REQUIRED)
target_link_libraries(day_07_main Threads::Threads)

# 测试目标
enable_testing()

add_test(NAME TestLeetCode42 COMMAND leetcode_42)
add_test(NAME TestLeetCode189 COMMAND leetcode_189)
