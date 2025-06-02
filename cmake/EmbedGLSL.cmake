# EmbedGLSL.cmake
# Converts GLSL source code to C header file with string literal

# Required variables:
# SHADER_NAME - Name for the shader string
# GLSL_FILE - Input GLSL source file
# OUTPUT_FILE - Output header file

if(NOT SHADER_NAME OR NOT GLSL_FILE OR NOT OUTPUT_FILE)
    message(FATAL_ERROR "SHADER_NAME, GLSL_FILE, and OUTPUT_FILE must be defined")
endif()

# Read the GLSL source file
file(READ "${GLSL_FILE}" GLSL_SOURCE)

# Escape special characters for C string literal
string(REPLACE "\\" "\\\\" GLSL_SOURCE "${GLSL_SOURCE}")
string(REPLACE "\"" "\\\"" GLSL_SOURCE "${GLSL_SOURCE}")
string(REPLACE "\n" "\\n\"\n\"" GLSL_SOURCE "${GLSL_SOURCE}")

# Generate header file content
set(HEADER_CONTENT "// Auto-generated GLSL shader source
// Source: ${GLSL_FILE}
// Generated: ${CMAKE_CURRENT_LIST_FILE}

#pragma once

static const char* ${SHADER_NAME}_source = 
\"${GLSL_SOURCE}\";
")

# Write header file
file(WRITE "${OUTPUT_FILE}" "${HEADER_CONTENT}")