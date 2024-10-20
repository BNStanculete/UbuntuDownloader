# Copyright 2024 HV0rtex

cmake_minimum_required(VERSION 3.11..3.22)

# Building GoogleTest
# -------------------
add_subdirectory("${PROJECT_SOURCE_DIR}/externals/googletest" "externals/googletest")

# Configuring GTest
# -----------------
mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

set_target_properties(gtest PROPERTIES FOLDER externals)
set_target_properties(gtest_main PROPERTIES FOLDER externals)
set_target_properties(gmock PROPERTIES FOLDER externals)
set_target_properties(gmock_main PROPERTIES FOLDER externals)

# Building UTs
# ------------

# Installing dependencies for CTs
# -------------------------------
install_python_requirements()
