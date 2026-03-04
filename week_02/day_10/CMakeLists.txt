cmake_minimum_required(VERSION 3.10)
project(day_10_weak_ptr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 可执行文件
add_executable(day_10_demo
    code/main.cpp
    code/cpp11_features/weak_ptr_demo.cpp
    code/emcpp/item21.cpp
    code/leetcode/0142_cycle_entrance/solution.cpp
    code/leetcode/0019_remove_nth/solution.cpp
)

target_link_libraries(day_10_demo PRIVATE pthread)

# 设置输出目录
set_target_properties(day_10_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
