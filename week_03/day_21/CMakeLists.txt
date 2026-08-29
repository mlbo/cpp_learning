cmake_minimum_required(VERSION 3.14)
project(day21_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if(ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

include(CTest)

function(add_day21_library target)
    add_library(${target} STATIC ${ARGN})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
endfunction()

add_day21_library(day21_stack_queue
    code/review/stack_queue_summary.cpp
)
add_day21_library(day21_lambda
    code/review/lambda_summary.cpp
)
add_day21_library(day21_emcpp
    code/review/emcpp_review.cpp
)
add_day21_library(day21_lc155
    code/leetcode/0155_min_stack/solution.cpp
)
add_day21_library(day21_lc150
    code/leetcode/0150_eval_rpn/solution.cpp
)

add_executable(day21_main code/main.cpp)
target_link_libraries(day21_main PRIVATE
    day21_stack_queue
    day21_lambda
    day21_emcpp
    day21_lc155
    day21_lc150
)

if(BUILD_TESTING)
    add_executable(day21_test_lc155
        code/leetcode/0155_min_stack/test.cpp
    )
    target_link_libraries(day21_test_lc155 PRIVATE day21_lc155)

    add_executable(day21_test_lc150
        code/leetcode/0150_eval_rpn/test.cpp
    )
    target_link_libraries(day21_test_lc150 PRIVATE day21_lc150)

    add_executable(day21_public_api_consumer code/tests/public_api_consumer.cpp)
    target_link_libraries(day21_public_api_consumer PRIVATE
        day21_stack_queue
        day21_lambda
        day21_emcpp
        day21_lc155
        day21_lc150
    )

    add_test(NAME Day21MinStack COMMAND day21_test_lc155)
    add_test(NAME Day21EvalRPN COMMAND day21_test_lc150)
    add_test(NAME Day21PublicApiConsumer COMMAND day21_public_api_consumer)
    add_test(NAME Day21SmokeRuns COMMAND day21_main)
endif()

message(STATUS "Day 21 targets consume reusable libraries through target_link_libraries")
