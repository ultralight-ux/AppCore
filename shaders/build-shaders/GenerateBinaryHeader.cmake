# GenerateBinaryHeader.cmake
# Generates a C header file from binary data

if(NOT DEFINED INPUT_FILE OR NOT DEFINED OUTPUT_FILE OR NOT DEFINED VARIABLE_NAME)
    message(FATAL_ERROR "INPUT_FILE, OUTPUT_FILE, and VARIABLE_NAME must be defined")
endif()

# Read binary file
file(READ "${INPUT_FILE}" FILE_CONTENTS HEX)
file(SIZE "${INPUT_FILE}" FILE_SIZE)

# Convert hex string to C array format
string(LENGTH "${FILE_CONTENTS}" HEX_LENGTH)
set(ARRAY_CONTENTS "")

# Process hex string in chunks
set(POS 0)
set(LINE_COUNT 0)
while(POS LESS HEX_LENGTH)
    # Extract up to 32 hex characters (16 bytes)
    math(EXPR CHUNK_LEN "${HEX_LENGTH} - ${POS}")
    if(CHUNK_LEN GREATER 32)
        set(CHUNK_LEN 32)
    endif()
    
    string(SUBSTRING "${FILE_CONTENTS}" ${POS} ${CHUNK_LEN} CHUNK)
    
    # Add indentation
    if(LINE_COUNT GREATER 0)
        string(APPEND ARRAY_CONTENTS "\n    ")
    else()
        string(APPEND ARRAY_CONTENTS "    ")
    endif()
    
    # Convert chunk to byte format
    set(BYTE_POS 0)
    while(BYTE_POS LESS CHUNK_LEN)
        string(SUBSTRING "${CHUNK}" ${BYTE_POS} 2 BYTE)
        string(APPEND ARRAY_CONTENTS "0x${BYTE}")
        
        math(EXPR NEXT_POS "${POS} + ${BYTE_POS} + 2")
        if(NEXT_POS LESS HEX_LENGTH)
            string(APPEND ARRAY_CONTENTS ", ")
        endif()
        
        math(EXPR BYTE_POS "${BYTE_POS} + 2")
    endwhile()
    
    math(EXPR POS "${POS} + ${CHUNK_LEN}")
    math(EXPR LINE_COUNT "${LINE_COUNT} + 1")
endwhile()

# Generate header content
set(HEADER_CONTENT "// Generated shader header for ${VARIABLE_NAME}
// Generated from: ${INPUT_FILE}
// Size: ${FILE_SIZE} bytes

#pragma once

static const unsigned char ${VARIABLE_NAME}_data[] = {
${ARRAY_CONTENTS}
};

static const unsigned int ${VARIABLE_NAME}_size = ${FILE_SIZE};
")

# Write header file
file(WRITE "${OUTPUT_FILE}" "${HEADER_CONTENT}")