# Copyright 2024 HV0rtex

cmake_minimum_required(VERSION 3.11..3.22)

# Function to call PIP to install dependencies
# --------------------------------------------
function(install_python_requirements)
    execute_process(COMMAND ${Python3_EXECUTABLE} -m pip install -r requirements.txt
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE PIP_RESULT
        OUTPUT_QUIET
    )

    if(NOT PIP_RESULT EQUAL "0")
        message(FATAL_ERROR "${Python3_EXECUTABLE} -m pip install -r requirements.txt failed with ${PIP_RESULT}, please check Python and pip installation.")
    endif()
endfunction()
