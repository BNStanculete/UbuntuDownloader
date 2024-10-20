# Copyright 2024 HV0rtex

cmake_minimum_required(VERSION 3.11..3.22)

# Function to create UTs (Unit Tests)
# -----------------------------------
function(create_test)
    # Configuring arguments
    # ---------------------
    set(OPTIONS "")
    set(ONE_VALUED_ARGUMENTS TARGET)
    set(MULTI_VALUED_ARGUMENTS SOURCES INCLUDE_DIRECTORIES LINK_LIBRARIES)

    cmake_parse_arguments(TEST "${OPTIONS}" "${ONE_VALUED_ARGUMENTS}" "${MULTI_VALUED_ARGUMENTS}" ${ARGN})

    # Create test executable
    # ----------------------
    add_executable(${TEST_TARGET} ${TEST_SOURCES})

    # Specifying includes
    # -------------------
    target_include_directories(${TEST_TARGET}
        PRIVATE
            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
            $<BUILD_INTERFACE:${TEST_INCLUDE_DIRECTORIES}>
    )

    # Linking dependencies
    # --------------------
    target_link_libraries(${TEST_TARGET}
        PRIVATE
            gtest gmock gtest_main ${TEST_LINK_LIBRARIES}
    )
    
    # Discover tests for the UT
    # -------------------------
    gtest_discover_tests(${TEST_TARGET}
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )

    # Configure test properties
    # -------------------------
    set_target_properties(${TEST_TARGET} PROPERTIES 
        FOLDER tests
    )
endfunction()