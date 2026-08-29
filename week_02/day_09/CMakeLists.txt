cmake_minimum_required(VERSION 3.14)
project(day_09_shared_ptr_and_list LANGUAGES CXX)

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
target_link_libraries(day_09_demo PRIVATE Threads::Threads)

# dummy_node.cpp 使用 shared_ptr 节点；与快慢指针示例的裸指针节点类型不同，
# 必须保持为独立程序，避免两个同名全局 ListNode 违反 ODR。
add_executable(day09_dummy_node_demo
    code/data_structure/dummy_main.cpp
    code/data_structure/dummy_node.cpp
)

# EMC++ 分离示例：保留原文件，同时纠正 Item 编号。
add_executable(day09_emcpp_details
    code/emcpp/details_main.cpp
    code/emcpp/item19_shared_ptr.cpp
    code/emcpp/item20_performance.cpp
)

# LeetCode 21：合并两个有序链表
add_executable(leetcode_0021_tests
    code/leetcode/0021_merge_lists/solution.cpp
    code/leetcode/0021_merge_lists/test.cpp
)
target_include_directories(leetcode_0021_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0021_merge_lists
)

# LeetCode 141：环形链表
add_executable(leetcode_0141_tests
    code/leetcode/0141_cycle/solution.cpp
    code/leetcode/0141_cycle/test_main.cpp
)
target_include_directories(leetcode_0141_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code/leetcode/0141_cycle
)

if(BUILD_TESTING)
    add_test(NAME day09_merge_lists COMMAND leetcode_0021_tests)
    add_test(NAME day09_cycle_detection COMMAND leetcode_0141_tests)
    add_test(NAME day09_dummy_node_smoke COMMAND day09_dummy_node_demo)
    add_test(NAME day09_emcpp_details_smoke COMMAND day09_emcpp_details)
    add_test(NAME day09_shared_ptr_smoke COMMAND day_09_demo)
endif()
