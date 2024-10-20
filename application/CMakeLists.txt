# Copyright 2024 HV0rtex

cmake_minimum_required(VERSION 3.11..3.22)

set(EXECUTABLE_HEADERS include/httpclient)

foreach(HEADER ${EXECUTABLE_HEADERS})
    configure_file(${HEADER}.hpp.in ${CMAKE_BINARY_DIR}/${HEADER}.hpp @ONLY)
endforeach()

add_executable(UbuntuDownloader
    source/main.cpp
    source/httpclient.cpp
)

target_include_directories(
    UbuntuDownloader
PRIVATE
    ${Boost_INCLUDE_DIR}
    ${JSON_INC_PATH}
    ${CMAKE_BINARY_DIR}/include
)

target_link_libraries(
    UbuntuDownloader
PRIVATE
    ${Boost_LIBRARIES}
    jsoncpp_lib
)