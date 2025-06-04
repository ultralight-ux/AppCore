# EmbedMetalBinary.cmake
# Converts Metal library (.metallib) to C binary header

if(NOT SHADER_NAME)
    message(FATAL_ERROR "SHADER_NAME not defined")
endif()

if(NOT METALLIB_FILE)
    message(FATAL_ERROR "METALLIB_FILE not defined")
endif()

if(NOT OUTPUT_FILE)
    message(FATAL_ERROR "OUTPUT_FILE not defined")
endif()

if(NOT EXISTS ${METALLIB_FILE})
    message(FATAL_ERROR "Metal library file not found: ${METALLIB_FILE}")
endif()

# Read binary data
file(READ ${METALLIB_FILE} METALLIB_CONTENT HEX)

# Get file size
file(SIZE ${METALLIB_FILE} METALLIB_SIZE)

# Convert hex string to C array format
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " METALLIB_ARRAY "${METALLIB_CONTENT}")
string(REGEX REPLACE ", $" "" METALLIB_ARRAY "${METALLIB_ARRAY}")

# Format array with line breaks (16 bytes per line)
string(REGEX REPLACE "((0x[0-9a-f][0-9a-f], ){15}0x[0-9a-f][0-9a-f], )" "\\1\n    " METALLIB_ARRAY_FORMATTED "${METALLIB_ARRAY}")

# Get base filename for comment
get_filename_component(METALLIB_FILENAME ${METALLIB_FILE} NAME)

# Generate header content
set(HEADER_CONTENT "// Generated Metal shader header for ${SHADER_NAME}
// Generated from: ${METALLIB_FILENAME}
// Size: ${METALLIB_SIZE} bytes

#pragma once

static const unsigned char ${SHADER_NAME}_data[] = {
    ${METALLIB_ARRAY_FORMATTED}
};

static const unsigned int ${SHADER_NAME}_size = ${METALLIB_SIZE};
")

# Write header file
file(WRITE ${OUTPUT_FILE} "${HEADER_CONTENT}")

message(STATUS "Generated Metal header: ${OUTPUT_FILE} (${METALLIB_SIZE} bytes)")