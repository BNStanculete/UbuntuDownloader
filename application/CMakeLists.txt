# Copyright 2024 HV0rtex

cmake_minimum_required(VERSION 3.11..3.22)

create_library(
    TARGET Dummy
    TYPE SHARED
    VERSION 1.0.0
    MAJOR_VERSION 1
    SOURCES "source/dummy.cpp"
    HEADERS "include/dummy"
)