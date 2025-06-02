# EmbedShader.cmake
# Converts binary shader bytecode to C header file

# Required variables:
# SHADER_NAME - Name for the shader array
# BYTECODE_FILE - Input binary file
# OUTPUT_FILE - Output header file

if(NOT SHADER_NAME OR NOT BYTECODE_FILE OR NOT OUTPUT_FILE)
    message(FATAL_ERROR "SHADER_NAME, BYTECODE_FILE, and OUTPUT_FILE must be defined")
endif()

# Read the binary file
file(READ "${BYTECODE_FILE}" SHADER_DATA HEX)

# Get file size
file(SIZE "${BYTECODE_FILE}" SHADER_SIZE)

# Convert hex string to C array
string(LENGTH "${SHADER_DATA}" HEX_LENGTH)
set(SHADER_ARRAY "")
set(BYTES_PER_LINE 16)
set(BYTE_COUNT 0)

# Process hex string two characters at a time
set(INDEX 0)
while(INDEX LESS HEX_LENGTH)
    # Get two hex characters (one byte)
    string(SUBSTRING "${SHADER_DATA}" ${INDEX} 2 HEX_BYTE)
    
    # Add to array
    if(BYTE_COUNT EQUAL 0)
        set(SHADER_ARRAY "${SHADER_ARRAY}    ")
    endif()
    
    set(SHADER_ARRAY "${SHADER_ARRAY}0x${HEX_BYTE}")
    
    math(EXPR INDEX "${INDEX} + 2")
    math(EXPR BYTE_COUNT "${BYTE_COUNT} + 1")
    
    # Add comma if not last byte
    if(INDEX LESS HEX_LENGTH)
        set(SHADER_ARRAY "${SHADER_ARRAY}, ")
    endif()
    
    # Add newline after BYTES_PER_LINE bytes
    if(BYTE_COUNT EQUAL BYTES_PER_LINE)
        set(SHADER_ARRAY "${SHADER_ARRAY}\n")
        set(BYTE_COUNT 0)
    endif()
endwhile()

# Generate header file content
set(HEADER_CONTENT "// Auto-generated shader bytecode
// Source: ${BYTECODE_FILE}
// Generated: ${CMAKE_CURRENT_LIST_FILE}

#pragma once

static const unsigned char ${SHADER_NAME}_data[] = {
${SHADER_ARRAY}
};

static const unsigned int ${SHADER_NAME}_size = ${SHADER_SIZE};
")

# Write header file
file(WRITE "${OUTPUT_FILE}" "${HEADER_CONTENT}")