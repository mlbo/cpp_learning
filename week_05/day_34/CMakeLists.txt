cmake_minimum_required(VERSION 3.14)
project(day_34_concurrency_integration LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
enable_testing()

function(add_day34_executable target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)
    target_compile_options(${target} PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror
    )
    target_link_libraries(${target} PRIVATE Threads::Threads)
    add_test(NAME ${target} COMMAND ${target})
endfunction()

add_day34_executable(day34_project_action code/main.cpp)
add_day34_executable(day34_process_thread_demo
    code/data_structure/process_thread_demo.cpp
)
add_day34_executable(day34_concurrency_demo
    code/cpp11_features/concurrency_demo.cpp
)
add_day34_executable(day34_emcpp_review
    code/emcpp/item35_40_review.cpp
)
add_day34_executable(day34_lc0236_demo
    code/leetcode/0236_lca/solution.cpp
)
add_day34_executable(day34_lc0236_contract_test
    code/leetcode/0236_lca/test.cpp
)
add_day34_executable(day34_lc0105_demo
    code/leetcode/0105_construct_tree/solution.cpp
)
add_day34_executable(day34_lc0105_contract_test
    code/leetcode/0105_construct_tree/test.cpp
)
