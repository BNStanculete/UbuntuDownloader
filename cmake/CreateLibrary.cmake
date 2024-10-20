# Copyright 2024 HV0rtex

cmake_minimum_required(VERSION 3.11..3.22)

# Configure postfix for Debug libraries
# -------------------------------------
set(CMAKE_DEBUG_POSTFIX -d)

# Function used to add a general purpose library
# ----------------------------------------------
function(create_library)

    # Configuring arguments
    # ---------------------
    set(OPTIONS "")
    set(ONE_VALUED_ARGUMENTS TARGET TYPE VERSION MAJOR_VERSION)
    set(MULTI_VALUED_ARGUMENTS SOURCES HEADERS INCLUDE_DIRECTORIES LINK_LIBRARIES)

    cmake_parse_arguments(LIBRARY "${OPTIONS}" "${ONE_VALUED_ARGUMENTS}" "${MULTI_VALUED_ARGUMENTS}" ${ARGN})

    # Creating library target
    # -----------------------
    add_library(${LIBRARY_TARGET} ${LIBRARY_TYPE}
        ${LIBRARY_SOURCES}
    )

    # Configuring properties
    # ----------------------
    set_target_properties(${LIBRARY_TARGET} PROPERTIES
        VERSION ${LIBRARY_VERSION}
        SOVERSION ${LIBRARY_MAJOR_VERSION}
        PUBLIC_HEADER include/${LIBRARY_TARGET}.hpp
        INTERFACE_${LIBRARY_TARGET}_MAJOR_VERSION ${LIBRARY_MAJOR_VERSION}
        COMPATIBLE_INTERFACE_STRING ${LIBRARY_TARGET}_MAJOR_VERSION)

    message(STATUS "Creating target ${LIBRARY_TARGET}")
    configure_compiler(${LIBRARY_TARGET})

    # Generating export header (Windows)
    # ----------------------------------
    set(CONFIGURE_EXPORT_MACRO "")
    set(CONFIGURE_EXPORT_INCLUDE "")
    set(CONFIGURE_LIBRARY_VERSION "${LIBRARY_VERSION}")

    if(WIN32)
        generate_export_header(${LIBRARY_TARGET})

        set(CONFIGURE_EXPORT_INCLUDE "#include <${LIBRARY_TARGET}_export>")
        set(CONFIGURE_EXPORT_MACRO "${LIBRARY_TARGET}_EXPORT")
    endif()

    foreach(HEADER ${LIBRARY_HEADERS})
        configure_file(${HEADER}.hpp.in ${CMAKE_BINARY_DIR}/${HEADER}.hpp @ONLY)
    endforeach()
    

    # Specifying includes
    # -------------------
    target_include_directories(${LIBRARY_TARGET}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
            $<BUILD_INTERFACE:${LIBRARY_INCLUDE_DIRECTORIES}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

    # Linking dependencies
    # --------------------
    target_link_libraries(${LIBRARY_TARGET}
        PRIVATE
            ${LIBRARY_LINK_LIBRARIES}
    )

    # Exporting library
    # -----------------
    install(
        TARGETS ${LIBRARY_TARGET}
        EXPORT AIDANTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

endfunction()