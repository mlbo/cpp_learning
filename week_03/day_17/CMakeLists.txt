cmake_minimum_required(VERSION 3.14)
project(day17_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

function(add_day17_library target)
    add_library(${target} STATIC ${ARGN})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
endfunction()

add_day17_library(day17_mono_stack
    code/algorithm/monotonic_stack_demo.cpp
)
add_day17_library(day17_function
    code/cpp11_features/function_demo.cpp
)
add_day17_library(day17_bind
    code/cpp11_features/bind_demo.cpp
)
add_day17_library(day17_item34
    code/emcpp/item34_lambda_vs_bind.cpp
)
add_day17_library(day17_lc739
    code/leetcode/0739_daily_temperatures/solution.cpp
)
add_day17_library(day17_lc496
    code/leetcode/0496_next_greater_element/solution.cpp
)

add_executable(day17_main code/main.cpp)
target_link_libraries(day17_main PRIVATE
    day17_mono_stack
    day17_function
    day17_bind
    day17_item34
    day17_lc739
    day17_lc496
)

if(BUILD_TESTING)
    add_executable(day17_public_api_consumer code/tests/public_api_consumer.cpp)
    target_link_libraries(day17_public_api_consumer PRIVATE
        day17_mono_stack
        day17_function
        day17_bind
        day17_item34
        day17_lc739
        day17_lc496
    )

    add_test(NAME day17_all COMMAND day17_main)
    add_test(NAME Day17PublicApiConsumer COMMAND day17_public_api_consumer)
endif()

message(STATUS "Day 17 targets consume reusable libraries through target_link_libraries")
