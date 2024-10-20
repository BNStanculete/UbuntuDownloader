# Copyright 2024 HV0rtex

cmake_minimum_required(VERSION 3.11..3.22)

# Locating Sphinx-Needs
# ---------------------
find_program(SPHINX_EXECUTABLE
    NAMES sphinx-build
    DOC "Path to sphinx-build executable"
)

find_package_handle_standard_args(Sphinx
    "Failed to find sphinx-build executable"
    SPHINX_EXECUTABLE
)