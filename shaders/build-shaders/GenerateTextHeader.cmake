# GenerateTextHeader.cmake
# Generates a C header file from text data (GLSL/Metal source) using raw string literals

if(NOT DEFINED INPUT_FILE OR NOT DEFINED OUTPUT_FILE OR NOT DEFINED VARIABLE_NAME)
    message(FATAL_ERROR "INPUT_FILE, OUTPUT_FILE, and VARIABLE_NAME must be defined")
endif()

# Read text file
file(read "${INPUT_FILE}" FILE_CONTENTS)

# Get input filename for comment
get_filename_component(INPUT_FILENAME "${INPUT_FILE}" NAME)

# Generate header content using raw string literal
set(HEADER_CONTENT "// Generated shader header for ${VARIABLE_NAME}
// Generated from: ${INPUT_FILENAME}

#pragma once

static const char* ${VARIABLE_NAME}_source = R\"GLSL(${FILE_CONTENTS})GLSL\";
")

# Write header file
file(WRITE "${OUTPUT_FILE}" "${HEADER_CONTENT}")