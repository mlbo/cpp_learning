cmake_minimum_required(VERSION 3.14)
project(day19_learning LANGUAGES CXX)

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

function(add_day19_library target)
    add_library(${target} STATIC ${ARGN})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
endfunction()

add_day19_library(day19_heap
    code/data_structure/heap_demo.cpp
)
add_day19_library(day19_pq
    code/data_structure/priority_queue_demo.cpp
)
add_day19_library(day19_lc215
    code/leetcode/0215_kth_largest/solution.cpp
)
add_day19_library(day19_lc347
    code/leetcode/0347_top_k_frequent/solution.cpp
)

add_executable(day19_main code/main.cpp)
target_link_libraries(day19_main PRIVATE
    day19_heap
    day19_pq
    day19_lc215
    day19_lc347
)

if(BUILD_TESTING)
    add_executable(day19_test_lc215
        code/leetcode/0215_kth_largest/test.cpp
    )
    target_link_libraries(day19_test_lc215 PRIVATE day19_lc215)

    add_executable(day19_test_lc347
        code/leetcode/0347_top_k_frequent/test.cpp
    )
    target_link_libraries(day19_test_lc347 PRIVATE day19_lc347)

    add_executable(day19_public_api_consumer code/tests/public_api_consumer.cpp)
    target_link_libraries(day19_public_api_consumer PRIVATE
        day19_heap
        day19_pq
        day19_lc215
        day19_lc347
    )

    add_test(NAME Day19KthLargest COMMAND day19_test_lc215)
    add_test(NAME Day19TopKFrequent COMMAND day19_test_lc347)
    add_test(NAME Day19PublicApiConsumer COMMAND day19_public_api_consumer)
    add_test(NAME Day19Smoke COMMAND day19_main)
endif()

message(STATUS "Day 19 targets consume reusable libraries through target_link_libraries")
