cmake_minimum_required(VERSION 3.10)
project(day_02_arrays_decltype CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
add_compile_options(-Wall -Wextra -Wpedantic)

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
