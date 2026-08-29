cmake_minimum_required(VERSION 3.16)
project(model_inference VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
find_package(Threads REQUIRED)

option(MODEL_INFERENCE_STRICT "Treat compiler warnings as errors" ON)
option(MODEL_INFERENCE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(MODEL_INFERENCE_TSAN "Enable ThreadSanitizer" OFF)

if(MODEL_INFERENCE_SANITIZERS AND MODEL_INFERENCE_TSAN)
    message(FATAL_ERROR "ASan/UBSan and TSan must use separate build directories")
endif()

add_library(model_inference_options INTERFACE)
target_compile_features(model_inference_options INTERFACE cxx_std_17)
target_include_directories(model_inference_options INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(model_inference_options INTERFACE Threads::Threads)

if(MSVC)
    target_compile_options(model_inference_options INTERFACE /W4)
    if(MODEL_INFERENCE_STRICT)
        target_compile_options(model_inference_options INTERFACE /WX)
    endif()
else()
    target_compile_options(model_inference_options INTERFACE
        -Wall -Wextra -Wpedantic
    )
    if(MODEL_INFERENCE_STRICT)
        target_compile_options(model_inference_options INTERFACE
            -Wconversion
            -Wsign-conversion
            -Wshadow
            -Werror
        )
    endif()
endif()

if(MODEL_INFERENCE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(model_inference_options INTERFACE
            -fsanitize=address,undefined -fno-omit-frame-pointer
        )
        target_link_options(model_inference_options INTERFACE
            -fsanitize=address,undefined -fno-omit-frame-pointer
        )
    else()
        message(FATAL_ERROR "MODEL_INFERENCE_SANITIZERS requires GCC or Clang")
    endif()
endif()

if(MODEL_INFERENCE_TSAN)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(model_inference_options INTERFACE
            -fsanitize=thread -fno-omit-frame-pointer
        )
        target_link_options(model_inference_options INTERFACE
            -fsanitize=thread -fno-omit-frame-pointer
        )
    else()
        message(FATAL_ERROR "MODEL_INFERENCE_TSAN requires GCC or Clang")
    endif()
endif()

add_executable(inference_demo src/main.cpp)
target_link_libraries(inference_demo PRIVATE model_inference_options)

if(BUILD_TESTING)
    add_executable(inference_contract_tests tests/contract_tests.cpp)
    target_link_libraries(inference_contract_tests PRIVATE model_inference_options)
    add_test(NAME inference_contracts COMMAND inference_contract_tests)
    set_tests_properties(inference_contracts PROPERTIES TIMEOUT 15)

    add_executable(inference_concurrency_tests tests/concurrency_tests.cpp)
    target_link_libraries(inference_concurrency_tests PRIVATE model_inference_options)
    add_test(NAME inference_concurrency COMMAND inference_concurrency_tests)
    set_tests_properties(inference_concurrency PROPERTIES TIMEOUT 30 LABELS "stress;concurrency")

    add_test(
        NAME inference_callback_destruction_rejected
        COMMAND ${CMAKE_COMMAND}
            -DPROGRAM=$<TARGET_FILE:inference_concurrency_tests>
            -DARGUMENT=--callback-destruction-probe
            -DEXPECTED_EXIT=86
            -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/expect_exit_code.cmake
    )
    set_tests_properties(inference_callback_destruction_rejected PROPERTIES
        TIMEOUT 15
        LABELS "lifecycle;concurrency;death-test"
    )

    add_test(
        NAME inference_batch_destruction_rejected
        COMMAND ${CMAKE_COMMAND}
            -DPROGRAM=$<TARGET_FILE:inference_concurrency_tests>
            -DARGUMENT=--batch-destruction-probe
            -DEXPECTED_EXIT=86
            -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/expect_exit_code.cmake
    )
    set_tests_properties(inference_batch_destruction_rejected PROPERTIES
        TIMEOUT 15
        LABELS "lifecycle;concurrency;death-test"
    )

    add_executable(inference_fault_injection_tests tests/fault_injection_tests.cpp)
    target_link_libraries(inference_fault_injection_tests PRIVATE model_inference_options)
    add_test(NAME inference_fault_injection COMMAND inference_fault_injection_tests)
    set_tests_properties(inference_fault_injection PROPERTIES TIMEOUT 15 LABELS "fault-injection")

    function(add_public_header_test target source)
        add_executable(${target} ${source})
        target_link_libraries(${target} PRIVATE model_inference_options)
        add_test(NAME ${target} COMMAND ${target})
        set_tests_properties(${target} PROPERTIES TIMEOUT 10 LABELS "public-headers")
    endfunction()

    add_public_header_test(header_tensor_self_contained
        tests/header_tensor_self_contained.cpp)
    add_public_header_test(header_layers_self_contained
        tests/header_layers_self_contained.cpp)
    add_public_header_test(header_inference_engine_self_contained
        tests/header_inference_engine_self_contained.cpp)
endif()

message(STATUS "")
message(STATUS "========== Model Inference Build ==========")
message(STATUS "C++ Standard: C++17")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Strict warnings: ${MODEL_INFERENCE_STRICT}")
message(STATUS "ASan/UBSan: ${MODEL_INFERENCE_SANITIZERS}")
message(STATUS "TSan: ${MODEL_INFERENCE_TSAN}")
message(STATUS "Tests: ${BUILD_TESTING}")
message(STATUS "===========================================")
