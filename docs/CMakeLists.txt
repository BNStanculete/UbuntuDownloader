# Copyright 2024 HV0rtex

cmake_minimum_required(VERSION 3.11..3.22)

# Installing Python modules
# -------------------------
install_python_requirements()

# Configuring Doxygen
# -------------------
set(DOXYGEN_INPUT_DIR ${PROJECT_BINARY_DIR}/include)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)

configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    MAIN_DEPENDENCY ${CMAKE_CURRENT_BINARY_DIR}/DoxyFile Doxyfile.in
    COMMENT "Generating documentation with Doxygen"
    VERBATIM
)

add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

# Configuring Sphinx
# ------------------
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
    COMMAND ${SPHINX_EXECUTABLE} -b html -Dbreathe_projects.AIDAN=${DOXYGEN_OUTPUT_DIR}/xml ${SPHINX_SOURCE} ${SPHINX_BUILD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/index.rst ${DOXYGEN_INDEX_FILE}
    MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
    COMMENT "Generating documentation with Sphinx"
)

add_custom_target(Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})

# Installing documentation
# ------------------------
install(DIRECTORY ${SPHINX_BUILD} DESTINATION ${CMAKE_INSTALL_DOCDIR})