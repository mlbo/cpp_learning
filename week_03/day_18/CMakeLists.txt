cmake_minimum_required(VERSION 3.14)
project(day18_learning LANGUAGES CXX)

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

function(add_day18_library target)
    add_library(${target} STATIC ${ARGN})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
endfunction()

add_day18_library(day18_call_stack
    code/low_level/call_stack_demo.cpp
)
add_day18_library(day18_stack_frame
    code/low_level/stack_frame.cpp
)
add_day18_library(day18_enum_class
    code/cpp11_features/enum_class_demo.cpp
)
add_day18_library(day18_enum_comparison
    code/cpp11_features/enum_vs_enum_class.cpp
)
add_day18_library(day18_item10
    code/emcpp/item10_enum_class.cpp
)
add_day18_library(day18_lc84
    code/leetcode/0084_largest_rectangle/solution.cpp
)
add_day18_library(day18_lc42
    code/leetcode/0042_trapping_rain_water/solution.cpp
)

add_executable(day18_main code/main.cpp)
target_link_libraries(day18_main PRIVATE
    day18_call_stack
    day18_stack_frame
    day18_enum_class
    day18_enum_comparison
    day18_item10
    day18_lc84
    day18_lc42
)

if(BUILD_TESTING)
    add_executable(day18_test_lc84
        code/leetcode/0084_largest_rectangle/test.cpp
    )
    target_link_libraries(day18_test_lc84 PRIVATE day18_lc84)

    add_executable(day18_test_lc42
        code/leetcode/0042_trapping_rain_water/test.cpp
    )
    target_link_libraries(day18_test_lc42 PRIVATE day18_lc42)

    add_executable(day18_public_api_consumer code/tests/public_api_consumer.cpp)
    target_link_libraries(day18_public_api_consumer PRIVATE
        day18_call_stack
        day18_stack_frame
        day18_enum_class
        day18_enum_comparison
        day18_item10
        day18_lc84
        day18_lc42
    )

    add_test(NAME Day18LargestRectangle COMMAND day18_test_lc84)
    add_test(NAME Day18TrappingRainWater COMMAND day18_test_lc42)
    add_test(NAME Day18PublicApiConsumer COMMAND day18_public_api_consumer)
    add_test(NAME Day18SmokeRuns COMMAND day18_main)
endif()

message(STATUS "Day 18 targets consume reusable libraries through target_link_libraries")
