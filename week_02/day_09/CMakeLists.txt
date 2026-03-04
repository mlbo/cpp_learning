cmake_minimum_required(VERSION 3.14)
project(day_09_shared_ptr_and_list LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
if(APPLE)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address,leak")
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# 包含公共头文件
include_directories(${CMAKE_SOURCE_DIR}/../../../common/include)

# 可执行文件
add_executable(day_09_demo
    code/main.cpp
    
    # 数据结构
    code/data_structure/fast_slow_pointer.cpp
    
    # C++11特性
    code/cpp11_features/shared_ptr_demo.cpp
    code/cpp11_features/shared_ptr_advanced.cpp
    code/cpp11_features/circular_reference.cpp
    
    # EMC++条款
    code/emcpp/item19_item20.cpp
    
)

target_compile_features(day_09_demo PRIVATE cxx_std_17)

# 添加测试
enable_testing()

add_test(NAME test_merge_lists COMMAND day_09_demo --gtest_filter=*MergeLists*)
add_test(NAME test_cycle COMMAND day_09_demo --gtest_filter=*Cycle*)
add_test(NAME test_shared_ptr COMMAND day_09_demo --gtest_filter=*SharedPtr*)
