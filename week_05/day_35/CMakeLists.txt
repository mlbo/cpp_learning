cmake_minimum_required(VERSION 3.14)
project(day_35_learning_evidence LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
enable_testing()

function(add_day35_executable target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)
    target_compile_options(${target} PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror
    )
    target_link_libraries(${target} PRIVATE Threads::Threads)
    add_test(NAME ${target} COMMAND ${target})
endfunction()

add_day35_executable(day35_project_contract code/main.cpp)
add_day35_executable(day35_knowledge_summary
    code/data_structure/knowledge_summary.cpp
)
add_day35_executable(day35_cpp17_summary
    code/cpp11_features/cpp11_summary.cpp
)
add_day35_executable(day35_emcpp_review
    code/emcpp/emcpp_review.cpp
)
add_day35_executable(day35_lc0297
    code/leetcode/0297_serialize_deserialize/solution.cpp
)
add_day35_executable(day35_lc0297_contract
    code/leetcode/0297_serialize_deserialize/contract_test.cpp
)
add_day35_executable(day35_lc0124
    code/leetcode/0124_max_path_sum/solution.cpp
)
add_day35_executable(day35_lc0124_contract
    code/leetcode/0124_max_path_sum/contract_test.cpp
)
