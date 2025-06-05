# GenerateMetalMasterHeader.cmake
# Generates a master header that includes all Metal shader headers

if(NOT METAL_HEADER_FILES)
    message(FATAL_ERROR "METAL_HEADER_FILES not defined")
endif()

if(NOT OUTPUT_FILE)
    message(FATAL_ERROR "OUTPUT_FILE not defined")
endif()

# The METAL_HEADER_FILES is already a proper CMake list when passed correctly
set(HEADER_LIST ${METAL_HEADER_FILES})

# Generate header content
set(HEADER_CONTENT "// Generated Metal shader master header - macOS platform
// Include this file to access all Metal compiled shaders

#pragma once

// Metal Shaders (compiled from source at build time)
")

# Add includes for each header file
foreach(HEADER_FILE ${HEADER_LIST})
    get_filename_component(HEADER_NAME ${HEADER_FILE} NAME)
    set(HEADER_CONTENT "${HEADER_CONTENT}#include \"${HEADER_NAME}\"\n")
endforeach()

# Write the master header
file(WRITE ${OUTPUT_FILE} "${HEADER_CONTENT}")

message(STATUS "Generated Metal master header: ${OUTPUT_FILE}")