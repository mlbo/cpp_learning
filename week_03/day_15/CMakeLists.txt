cmake_minimum_required(VERSION 3.14)
project(day15_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

# 每个库自己公开消费者需要的头文件搜索路径；不使用目录级全局 include 路径。
function(add_day15_library target)
    add_library(${target} STATIC ${ARGN})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
endfunction()

add_day15_library(day15_stack_demo
    code/data_structure/stack_demo.cpp
)
add_day15_library(day15_lambda_demo
    code/cpp11_features/lambda_demo.cpp
)
add_day15_library(day15_lambda_capture
    code/cpp11_features/lambda_capture_demo.cpp
)
add_day15_library(day15_item31
    code/emcpp/item31_avoid_default_capture.cpp
)
add_day15_library(day15_lc20
    code/leetcode/0020_valid_parentheses/solution.cpp
)
add_day15_library(day15_lc1047
    code/leetcode/1047_remove_adjacent_duplicates/solution.cpp
)

# 主程序只编译入口文件，所有实现都通过库边界消费。
add_executable(day15_main code/main.cpp)
target_link_libraries(day15_main PRIVATE
    day15_stack_demo
    day15_lambda_demo
    day15_lambda_capture
    day15_item31
    day15_lc20
    day15_lc1047
)

if(BUILD_TESTING)
    add_executable(day15_public_api_consumer code/tests/public_api_consumer.cpp)
    target_link_libraries(day15_public_api_consumer PRIVATE
        day15_stack_demo
        day15_lambda_demo
        day15_lambda_capture
        day15_item31
        day15_lc20
        day15_lc1047
    )

    add_test(NAME day15_all COMMAND day15_main)
    add_test(NAME Day15PublicApiConsumer COMMAND day15_public_api_consumer)
endif()

message(STATUS "Day 15 targets consume reusable libraries through target_link_libraries")
