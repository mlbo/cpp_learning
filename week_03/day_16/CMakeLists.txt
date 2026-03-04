cmake_minimum_required(VERSION 3.14)
project(day16_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)  # C++14 for generic lambda
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# 数据结构演示
add_executable(day16_queue_demo
    code/data_structure/queue_demo.cpp
)

# Lambda泛型演示
add_executable(day16_lambda_generic
    code/cpp11_features/lambda_generic_demo.cpp
)

# Lambda初始化捕获演示
add_executable(day16_lambda_init
    code/cpp11_features/lambda_init_capture_demo.cpp
)

# EMC++ Item 32-33演示
add_executable(day16_item32_33
    code/emcpp/item32_item33.cpp
)

# LeetCode 232
add_executable(day16_lc232
    code/leetcode/0232_implement_queue_using_stacks/solution.cpp
)

# LeetCode 225
add_executable(day16_lc225
    code/leetcode/0225_implement_stack_using_queues/solution.cpp
)

# 主程序
add_executable(day16_main
    code/main.cpp
    code/data_structure/queue_demo.cpp
    code/cpp11_features/lambda_generic_demo.cpp
    code/cpp11_features/lambda_init_capture_demo.cpp
    code/emcpp/item32_item33.cpp
    code/leetcode/0232_implement_queue_using_stacks/solution.cpp
    code/leetcode/0225_implement_stack_using_queues/solution.cpp
)

message(STATUS "Day 16 targets:")
message(STATUS "  day16_main        - 运行所有演示")
message(STATUS "  day16_queue_demo  - 队列演示")
message(STATUS "  day16_lambda_generic - 泛型Lambda")
message(STATUS "  day16_lambda_init - 初始化捕获")
message(STATUS "  day16_item32_33   - EMC++ Item 32-33")
message(STATUS "  day16_lc232       - LeetCode 232")
message(STATUS "  day16_lc225       - LeetCode 225")
