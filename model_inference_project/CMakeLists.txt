# C++ Model Inference Project
# CMake Configuration
cmake_minimum_required(VERSION 3.10)
project(model_inference VERSION 1.0.0 LANGUAGES CXX)

# C++11 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Find Threads library
find_package(Threads REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Main executable
add_executable(inference_demo
    src/main.cpp
)

# Link with threads
target_link_libraries(inference_demo PRIVATE Threads::Threads)

# Print configuration
message(STATUS "")
message(STATUS "========== Build Configuration ==========")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "==========================================")
message(STATUS "")
