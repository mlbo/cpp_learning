if(NOT DEFINED PROGRAM OR NOT DEFINED ARGUMENT OR NOT DEFINED EXPECTED_EXIT)
    message(FATAL_ERROR "PROGRAM, ARGUMENT and EXPECTED_EXIT are required")
endif()

execute_process(
    COMMAND "${PROGRAM}" "${ARGUMENT}"
    RESULT_VARIABLE actual_exit
    OUTPUT_VARIABLE child_stdout
    ERROR_VARIABLE child_stderr
)

if(NOT "${actual_exit}" STREQUAL "${EXPECTED_EXIT}")
    message(FATAL_ERROR
        "callback destruction probe exit=${actual_exit}, expected=${EXPECTED_EXIT}\n"
        "stdout:\n${child_stdout}\n"
        "stderr:\n${child_stderr}")
endif()
