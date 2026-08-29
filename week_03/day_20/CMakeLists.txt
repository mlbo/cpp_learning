cmake_minimum_required(VERSION 3.14)
project(day20_learning LANGUAGES CXX)

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

function(add_day20_library target)
    add_library(${target} STATIC ${ARGN})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
endfunction()

add_day20_library(day20_tree
    code/data_structure/tree_node.cpp
)
add_day20_library(day20_bfs_template
    code/algorithm/bfs_template.cpp
)
add_day20_library(day20_lc102
    code/leetcode/0102_level_order/solution.cpp
)
add_day20_library(day20_lc107
    code/leetcode/0107_level_order_bottom/solution.cpp
)

# 该文件拥有自己的 main，只消费树工具库，不与课程总入口混编。
add_executable(day20_bfs_demo code/algorithm/bfs_demo.cpp)
target_link_libraries(day20_bfs_demo PRIVATE day20_tree)

add_executable(day20_main code/main.cpp)
target_link_libraries(day20_main PRIVATE
    day20_bfs_template
    day20_lc102
    day20_lc107
)

if(BUILD_TESTING)
    add_executable(day20_test_lc102
        code/leetcode/0102_level_order/test.cpp
    )
    target_link_libraries(day20_test_lc102 PRIVATE day20_lc102)

    add_executable(day20_test_lc107
        code/leetcode/0107_level_order_bottom/test.cpp
    )
    target_link_libraries(day20_test_lc107 PRIVATE day20_lc107)

    add_executable(day20_test_bfs_template
        code/algorithm/bfs_template_test.cpp
    )
    target_link_libraries(day20_test_bfs_template PRIVATE day20_bfs_template)

    add_executable(day20_public_api_consumer code/tests/public_api_consumer.cpp)
    target_link_libraries(day20_public_api_consumer PRIVATE
        day20_tree
        day20_bfs_template
        day20_lc102
        day20_lc107
    )

    add_test(NAME Day20LevelOrder COMMAND day20_test_lc102)
    add_test(NAME Day20LevelOrderBottom COMMAND day20_test_lc107)
    add_test(NAME Day20BfsTemplate COMMAND day20_test_bfs_template)
    add_test(NAME Day20BfsStandalone COMMAND day20_bfs_demo)
    add_test(NAME Day20PublicApiConsumer COMMAND day20_public_api_consumer)
    add_test(NAME Day20SmokeRuns COMMAND day20_main)
endif()

message(STATUS "Day 20 tree, BFS and LeetCode targets expose explicit consumer boundaries")
