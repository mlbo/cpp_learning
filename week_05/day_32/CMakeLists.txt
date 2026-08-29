cmake_minimum_required(VERSION 3.14)
project(day_32_dfs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Werror
    )
endif()

find_package(Threads REQUIRED)
include(CTest)

function(day32_add_test target source)
    add_executable(${target} ${source})
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)
    add_test(NAME ${target} COMMAND ${target})
    set_tests_properties(${target} PROPERTIES TIMEOUT 10)
endfunction()

day32_add_test(day32_main code/main.cpp)
target_link_libraries(day32_main PRIVATE Threads::Threads)

day32_add_test(day32_dfs_demo code/data_structure/dfs_demo.cpp)

day32_add_test(day32_atomic_demo code/cpp11_features/atomic_demo.cpp)
target_link_libraries(day32_atomic_demo PRIVATE Threads::Threads)

day32_add_test(day32_item39_40 code/emcpp/item39_40_concurrent.cpp)
target_link_libraries(day32_item39_40 PRIVATE Threads::Threads)

day32_add_test(day32_lc0104_demo code/leetcode/0104_maximum_depth/solution.cpp)
day32_add_test(day32_lc0104_test code/leetcode/0104_maximum_depth/test.cpp)
day32_add_test(day32_lc0111_demo code/leetcode/0111_minimum_depth/solution.cpp)
day32_add_test(day32_lc0111_test code/leetcode/0111_minimum_depth/test.cpp)
