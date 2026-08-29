cmake_minimum_required(VERSION 3.14)
project(day16_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

function(add_day16_library target)
    add_library(${target} STATIC ${ARGN})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
endfunction()

add_day16_library(day16_queue_demo
    code/data_structure/queue_demo.cpp
)
add_day16_library(day16_lambda_generic
    code/cpp11_features/lambda_generic_demo.cpp
)
add_day16_library(day16_lambda_init
    code/cpp11_features/lambda_init_capture_demo.cpp
)
add_day16_library(day16_item32_33
    code/emcpp/item32_item33.cpp
)
add_day16_library(day16_lc232
    code/leetcode/0232_implement_queue_using_stacks/solution.cpp
)
add_day16_library(day16_lc225
    code/leetcode/0225_implement_stack_using_queues/solution.cpp
)

add_executable(day16_main code/main.cpp)
target_link_libraries(day16_main PRIVATE
    day16_queue_demo
    day16_lambda_generic
    day16_lambda_init
    day16_item32_33
    day16_lc232
    day16_lc225
)

if(BUILD_TESTING)
    add_executable(day16_lc232_tests
        code/leetcode/0232_implement_queue_using_stacks/test.cpp
    )
    target_link_libraries(day16_lc232_tests PRIVATE day16_lc232)

    add_executable(day16_public_api_consumer code/tests/public_api_consumer.cpp)
    target_link_libraries(day16_public_api_consumer PRIVATE
        day16_queue_demo
        day16_lambda_generic
        day16_lambda_init
        day16_item32_33
        day16_lc232
        day16_lc225
    )

    add_test(NAME day16_all COMMAND day16_main)
    add_test(NAME day16_lc232_detailed COMMAND day16_lc232_tests)
    add_test(NAME Day16PublicApiConsumer COMMAND day16_public_api_consumer)
endif()

message(STATUS "Day 16 targets consume reusable libraries through target_link_libraries")
