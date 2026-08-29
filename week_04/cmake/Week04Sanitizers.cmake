function(week04_require_complete_tree week04_root)
    set(week04_missing_paths)
    if(NOT EXISTS "${week04_root}/verify_readme_programs.sh")
        list(APPEND week04_missing_paths "verify_readme_programs.sh")
    endif()
    foreach(week04_day RANGE 22 28)
        if(NOT EXISTS "${week04_root}/day_${week04_day}/README.md")
            list(APPEND week04_missing_paths "day_${week04_day}/README.md")
        endif()
    endforeach()

    if(week04_missing_paths)
        list(JOIN week04_missing_paths ", " week04_missing_summary)
        message(FATAL_ERROR
            "Week 4 的可搬迁与质量门禁边界是整棵 week_04；"
            "当前目录缺少: ${week04_missing_summary}。"
            "请复制完整 week_04，不要只复制单个 Day。")
    endif()
endfunction()

function(week04_enable_sanitizers)
    option(ENABLE_SANITIZERS "Enable both AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
    option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
    option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

    set(week04_sanitizers)
    if(ENABLE_SANITIZERS OR ENABLE_ASAN)
        list(APPEND week04_sanitizers address)
    endif()
    if(ENABLE_SANITIZERS OR ENABLE_UBSAN)
        list(APPEND week04_sanitizers undefined)
    endif()

    if(week04_sanitizers)
        if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
            message(FATAL_ERROR
                "Week 4 sanitizer options currently support GCC and Clang family compilers only")
        endif()
        list(JOIN week04_sanitizers "," week04_sanitizer_list)
        add_compile_options(
            "-fsanitize=${week04_sanitizer_list}"
            -fno-omit-frame-pointer
        )
        add_link_options(
            "-fsanitize=${week04_sanitizer_list}"
            -fno-omit-frame-pointer
        )
        message(STATUS "Week 4 sanitizers: ${week04_sanitizer_list}")
    endif()
endfunction()
