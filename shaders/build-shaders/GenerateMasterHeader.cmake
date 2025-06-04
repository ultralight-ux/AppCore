# GenerateMasterHeader.cmake
# Generates a master header file that includes all shader headers

if(NOT DEFINED VERTEX_SHADERS OR NOT DEFINED PIXEL_SHADERS OR NOT DEFINED OUTPUT_FILE)
    message(FATAL_ERROR "VERTEX_SHADERS, PIXEL_SHADERS, and OUTPUT_FILE must be defined")
endif()

# Convert semicolon-separated lists
string(REPLACE ";" " " VERTEX_LIST "${VERTEX_SHADERS}")
string(REPLACE ";" " " PIXEL_LIST "${PIXEL_SHADERS}")

# Start header content
set(HEADER_CONTENT "// Generated master shader header
// Include this file to access all compiled shaders for the current platform

#pragma once

")

# Add D3D11 headers (Windows only)
string(APPEND HEADER_CONTENT "#ifdef _WIN32\n")
string(APPEND HEADER_CONTENT "// D3D11 Shaders (Windows)\n")

# Add vertex shader includes
foreach(SHADER ${VERTEX_SHADERS})
    string(APPEND HEADER_CONTENT "#include \"d3d11/${SHADER}_vs.h\"\n")
endforeach()

# Add pixel shader includes
foreach(SHADER ${PIXEL_SHADERS})
    string(APPEND HEADER_CONTENT "#include \"d3d11/${SHADER}_ps.h\"\n")
endforeach()

string(APPEND HEADER_CONTENT "#endif // _WIN32\n\n")

# Add OpenGL headers (Linux and others)
string(APPEND HEADER_CONTENT "#if defined(__linux__) || defined(__unix__)\n")
string(APPEND HEADER_CONTENT "// GLSL Shaders (OpenGL)\n")

# Add vertex shader includes
foreach(SHADER ${VERTEX_SHADERS})
    string(APPEND HEADER_CONTENT "#include \"glsl/${SHADER}_vs.h\"\n")
endforeach()

# Add fragment shader includes
foreach(SHADER ${PIXEL_SHADERS})
    string(APPEND HEADER_CONTENT "#include \"glsl/${SHADER}_fs.h\"\n")
endforeach()

string(APPEND HEADER_CONTENT "#endif // __linux__ || __unix__\n\n")

# Add Metal headers (macOS)
string(APPEND HEADER_CONTENT "#ifdef __APPLE__\n")
string(APPEND HEADER_CONTENT "// Metal Shaders (macOS)\n")

# Add vertex shader includes
foreach(SHADER ${VERTEX_SHADERS})
    string(APPEND HEADER_CONTENT "#include \"metal/${SHADER}_vs.h\"\n")
endforeach()

# Add pixel shader includes
foreach(SHADER ${PIXEL_SHADERS})
    string(APPEND HEADER_CONTENT "#include \"metal/${SHADER}_ps.h\"\n")
endforeach()

string(APPEND HEADER_CONTENT "#endif // __APPLE__\n")

# Write header file
file(WRITE "${OUTPUT_FILE}" "${HEADER_CONTENT}")