# CMakeLists.txt for offline shader building  
# This is a standalone build system for compiling shaders without building AppCore
# Uses local tools in ../tools/ directory

cmake_minimum_required(VERSION 3.10)
project(AppCoreShaderBuild)

# Options
option(SHADER_DEBUG "Enable shader debugging" OFF)

# Output directory
set(SHADER_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../generated" CACHE PATH "Output directory for compiled shaders")

# Local tool paths - all tools are in ../tools/
set(TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../tools")
set(FXC_EXECUTABLE "${TOOLS_DIR}/fxc.exe")
set(DXC_EXECUTABLE "${TOOLS_DIR}/dxc/bin/x64/dxc.exe")
set(SPIRV_CROSS_EXECUTABLE "${TOOLS_DIR}/spirv-cross.exe")

# Verify tools exist
if(NOT EXISTS "${FXC_EXECUTABLE}")
    message(FATAL_ERROR "FXC not found at: ${FXC_EXECUTABLE}")
endif()

if(NOT EXISTS "${SPIRV_CROSS_EXECUTABLE}")
    message(FATAL_ERROR "SPIRV-Cross not found at: ${SPIRV_CROSS_EXECUTABLE}")
endif()

# DXC will be downloaded automatically if not present
if(NOT EXISTS "${DXC_EXECUTABLE}")
    message(STATUS "DXC not found, will be downloaded from GitHub releases")
endif()

message(STATUS "Using local tools:")
message(STATUS "  FXC: ${FXC_EXECUTABLE}")
message(STATUS "  DXC: ${DXC_EXECUTABLE}")
message(STATUS "  SPIRV-Cross: ${SPIRV_CROSS_EXECUTABLE}")

# Shader lists
set(VERTEX_SHADERS
    vertex_path
    vertex_quad
)

set(PIXEL_SHADERS
    fill
    fill_path
    filter_basic
    filter_blur
    filter_dropshadow
)

# Include directories for shaders
set(SHADER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../hlsl")

# Create output directories
file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}")
file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}/temp")           # Temporary files (not committed)
file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}/metal")          # Metal source files (for macOS)
file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}/headers")
file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}/headers/d3d11")  # D3D11 binary headers (Windows)
file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}/headers/glsl")   # GLSL text headers (Linux)

# Function to add shader compilation target
function(add_shader_target SHADER_NAME SHADER_TYPE)
    set(SHADER_FILE "${SHADER_INCLUDE_DIR}/${SHADER_NAME}.hlsl")
    
    if(${SHADER_TYPE} STREQUAL "VERTEX")
        set(PROFILE_D3D "vs_4_0")
        set(PROFILE_DXC "vs_6_0")  # DXC minimum is SM 6.0
        set(SUFFIX "_vs")
    else()
        set(PROFILE_D3D "ps_4_0")
        set(PROFILE_DXC "ps_6_0")  # DXC minimum is SM 6.0
        set(SUFFIX "_ps")
    endif()
    
    # D3D11 compilation (Windows only)
    if(WIN32)
        set(D3D11_OUTPUT "${SHADER_OUTPUT_DIR}/temp/${SHADER_NAME}${SUFFIX}.cso")
        set(D3D11_HEADER "${SHADER_OUTPUT_DIR}/headers/d3d11/${SHADER_NAME}${SUFFIX}.h")
        
        if(SHADER_DEBUG)
            set(FXC_FLAGS /Od /Zi)
        else()
            set(FXC_FLAGS /O2)
        endif()
        
        add_custom_command(
            OUTPUT ${D3D11_OUTPUT}
            COMMAND ${FXC_EXECUTABLE}
                /T ${PROFILE_D3D}
                /E main
                ${FXC_FLAGS}
                /I "${SHADER_INCLUDE_DIR}"
                /Fo "${D3D11_OUTPUT}"
                "${SHADER_FILE}"
            DEPENDS ${SHADER_FILE}
                    "${SHADER_INCLUDE_DIR}/common.hlsli"
                    "${SHADER_INCLUDE_DIR}/clip.hlsli"
            COMMENT "Compiling D3D11 shader: ${SHADER_NAME}"
        )
        
        add_custom_command(
            OUTPUT ${D3D11_HEADER}
            COMMAND ${CMAKE_COMMAND}
                -DINPUT_FILE="${D3D11_OUTPUT}"
                -DOUTPUT_FILE="${D3D11_HEADER}"
                -DVARIABLE_NAME="${SHADER_NAME}${SUFFIX}"
                -P "${CMAKE_CURRENT_SOURCE_DIR}/GenerateBinaryHeader.cmake"
            DEPENDS ${D3D11_OUTPUT}
            COMMENT "Generating D3D11 header: ${SHADER_NAME}"
        )
        
        list(APPEND ALL_OUTPUTS ${D3D11_HEADER})
    endif()
    
    # SPIR-V compilation (temporary file)
    set(SPIRV_OUTPUT "${SHADER_OUTPUT_DIR}/temp/${SHADER_NAME}.spv")
    
    if(SHADER_DEBUG)
        set(DXC_FLAGS -Od)
    else()
        set(DXC_FLAGS -O3)
    endif()
    
    add_custom_command(
        OUTPUT ${SPIRV_OUTPUT}
        COMMAND ${DXC_EXECUTABLE}
            -T ${PROFILE_DXC}
            -E main
            -spirv
            -fspv-target-env=vulkan1.1
            ${DXC_FLAGS}
            -I "${SHADER_INCLUDE_DIR}"
            -Fo "${SPIRV_OUTPUT}"
            "${SHADER_FILE}"
        DEPENDS ${SHADER_FILE}
                "${SHADER_INCLUDE_DIR}/common.hlsli"
                "${SHADER_INCLUDE_DIR}/clip.hlsli"
        COMMENT "Compiling SPIR-V: ${SHADER_NAME}"
    )
    
    # GLSL cross-compilation (temporary file)
    set(GLSL_OUTPUT "${SHADER_OUTPUT_DIR}/temp/${SHADER_NAME}.glsl")
    set(GLSL_SUFFIX "${SUFFIX}")
    if(${SHADER_TYPE} STREQUAL "PIXEL")
        string(REPLACE "_ps" "_fs" GLSL_SUFFIX "${SUFFIX}")
    endif()
    set(GLSL_HEADER "${SHADER_OUTPUT_DIR}/headers/glsl/${SHADER_NAME}${GLSL_SUFFIX}.h")
    
    add_custom_command(
        OUTPUT ${GLSL_OUTPUT}
        COMMAND ${SPIRV_CROSS_EXECUTABLE}
            "${SPIRV_OUTPUT}"
            --version 330
            --no-420pack-extension
            --remove-unused-variables
            --output "${GLSL_OUTPUT}"
        DEPENDS ${SPIRV_OUTPUT}
        COMMENT "Cross-compiling to GLSL: ${SHADER_NAME}"
    )
    
    add_custom_command(
        OUTPUT ${GLSL_HEADER}
        COMMAND ${CMAKE_COMMAND}
            -DINPUT_FILE="${GLSL_OUTPUT}"
            -DOUTPUT_FILE="${GLSL_HEADER}"
            -DVARIABLE_NAME="${SHADER_NAME}${GLSL_SUFFIX}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/GenerateTextHeader.cmake"
        DEPENDS ${GLSL_OUTPUT}
        COMMENT "Generating GLSL header: ${SHADER_NAME}"
    )
    
    list(APPEND ALL_OUTPUTS ${GLSL_HEADER})
    
    # Metal cross-compilation (source files only - compiled on macOS)
    set(METAL_OUTPUT "${SHADER_OUTPUT_DIR}/metal/${SHADER_NAME}.metal")
    
    add_custom_command(
        OUTPUT ${METAL_OUTPUT}
        COMMAND ${SPIRV_CROSS_EXECUTABLE}
            "${SPIRV_OUTPUT}"
            --msl
            --msl-version 20100
            --msl-argument-buffers
            --output "${METAL_OUTPUT}"
        DEPENDS ${SPIRV_OUTPUT}
        COMMENT "Cross-compiling to Metal: ${SHADER_NAME}"
    )
    
    list(APPEND ALL_OUTPUTS ${METAL_OUTPUT})
    
    # Store outputs in parent scope
    set(ALL_OUTPUTS ${ALL_OUTPUTS} PARENT_SCOPE)
endfunction()

# Add all shader targets
set(ALL_OUTPUTS)

foreach(SHADER ${VERTEX_SHADERS})
    add_shader_target(${SHADER} VERTEX)
endforeach()

foreach(SHADER ${PIXEL_SHADERS})
    add_shader_target(${SHADER} PIXEL)
endforeach()

# Generate master header
set(MASTER_HEADER "${SHADER_OUTPUT_DIR}/headers/compiled_shaders.h")
add_custom_command(
    OUTPUT ${MASTER_HEADER}
    COMMAND ${CMAKE_COMMAND}
        -DVERTEX_SHADERS="${VERTEX_SHADERS}"
        -DPIXEL_SHADERS="${PIXEL_SHADERS}"
        -DOUTPUT_FILE="${MASTER_HEADER}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/GenerateMasterHeader.cmake"
    DEPENDS ${ALL_OUTPUTS}
    COMMENT "Generating master header"
)

# Main target
add_custom_target(compile_shaders ALL
    DEPENDS ${ALL_OUTPUTS} ${MASTER_HEADER}
)

# Install target (optional)
install(DIRECTORY "${SHADER_OUTPUT_DIR}/headers/"
    DESTINATION include/AppCore/shaders
    FILES_MATCHING PATTERN "*.h"
)

message(STATUS "Shader build configured. Output will be in: ${SHADER_OUTPUT_DIR}")
message(STATUS "Build with: cmake --build . --target compile_shaders")