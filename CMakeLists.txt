# Copyright 2024 HV0rtex

cmake_minimum_required(VERSION 3.11..3.22)

# Declaring Project
# -----------------
project(UbuntuDownloader
    DESCRIPTION
        "A tool for managing Ubuntu releases"
    VERSION 
        "0.0.1"
    LANGUAGES
        "CXX"
)

# Declaring project options
# -------------------------
option(BUILD_TESTS "Build the whole test suite" ON)
option(BUILD_DOCS "Build the documentation" ON)

# Sourcing CMake modules
# ----------------------
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/" ${CMAKE_MODULES_PATH})

# Including CMake utilitaries
# ---------------------------
include(CMakePackageConfigHelpers)
include(CMakeFindDependencyMacro)
include(InstallRequirements)
include(ConfigureCompiler)
include(SyncSubmodules)
include(GNUInstallDirs)

if(WIN32)
    include(GenerateExportHeader)

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

# Linking external libraries
# --------------------------
if(${BUILD_DOCS} OR ${BUILD_TESTS})
    find_package(Python3 REQUIRED)
endif()

if(${BUILD_DOCS})
    find_package(PackageHandleStandardArgs REQUIRED)
    find_package(Doxygen REQUIRED)
    find_package(Sphinx REQUIRED)
endif()

find_package(Boost REQUIRED)

# Building JSONcpp
# -------------------
add_subdirectory("${PROJECT_SOURCE_DIR}/externals/jsoncpp" "externals/jsoncpp")

set_target_properties(jsoncpp_lib PROPERTIES FOLDER externals)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)

# Adding project targets
# ----------------------
add_subdirectory(application)

if(${BUILD_TESTS})
    enable_testing()

    include(GoogleTest)
    include(CreateUT)

    add_subdirectory(tests)
endif()

if(${BUILD_DOCS})
    add_subdirectory(docs)
endif()