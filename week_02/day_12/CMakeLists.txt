cmake_minimum_required(VERSION 3.14)
project(day_12 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
add_compile_options(-Wall -Wextra -Wpedantic)

# 源文件收集
set(CPP11_SOURCES
    code/cpp11_features/smart_ptr_guide.cpp
    code/cpp11_features/raw_vs_smart.cpp
    code/cpp11_features/custom_deleter.cpp
)

set(LOW_LEVEL_SOURCES
    code/low_level/heap_memory.cpp
    code/low_level/raii_demo.cpp
    code/low_level/memory_leak.cpp
)

set(LEETCODE_SOURCES
    code/leetcode/0024_swap_pairs/solution.cpp
    code/leetcode/0024_swap_pairs/test.cpp
    code/leetcode/0025_reverse_k/solution.cpp
)

# 主可执行文件
add_executable(day_12
    code/main.cpp
    ${CPP11_SOURCES}
    ${LOW_LEVEL_SOURCES}
    ${LEETCODE_SOURCES}
)

# 线程支持 (如果需要)
find_package(Threads QUIET)
if(Threads_FOUND)
    target_link_libraries(day_12 PRIVATE Threads::Threads)
endif()

# 启用 Sanitizers (调试用)
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
if(ENABLE_ASAN)
    target_compile_options(day_12 PRIVATE -fsanitize=address -fno-omit-frame-pointer -g)
    target_link_options(day_12 PRIVATE -fsanitize=address)
endif()
