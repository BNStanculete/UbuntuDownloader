# Copyright 2024 HV0rtex

cmake_minimum_required(VERSION 3.11..3.22)

# Configuring C++ Standard
# ------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuring build flags based on OS
# -----------------------------------
function(configure_compiler COMPILER_TARGET)
    if(MSVC)
        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            target_compile_options(${COMPILER_TARGET}
                PUBLIC
                    -analyze
                    -fastfail
                    -fp:except
                    -sdl
                    -W4
                    -WX
                    -Zc:enumTypes
                    -Zc:externC
                    -Zc:forScope
                    -Zc:inline
                    -Zc:noexceptTypes
                    -Zc:templateScope
                    -Zc:ternary
                    -Zc:threadSafeInit
                    -Zc:throwingNew
                    -permissive
            )
        else()
            target_compile_options(${COMPILER_TARGET}
                PUBLIC
                    -Oi
                    -O2
                    -GA
                    -GL
                    -Gw
                    -Qfast_transcendentals
                    -sdl
            )
        endif()
    elseif(UNIX)
        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            target_compile_options(${COMPILER_TARGET}
                PUBLIC
                    -Wpedantic
                    -Wconversion
                    -Wcast-align
                    -Wunused
                    -Wshadow
                    -Wold-style-cast
                    -Wpointer-arith
                    -Wcast-qual
                    -Wno-missing-braces
                    -Wall
                    -Wextra
                    -Werror
            )
        else()
            target_compile_options(${COMPILER_TARGET}
                PUBLIC
                    -Ofast
                    -funroll-loops
            )
        endif()
    elseif(APPLE)
        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

        else()

        endif()
    endif()
endfunction()
