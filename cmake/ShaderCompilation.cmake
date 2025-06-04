# ShaderCompilation.cmake
# Unified shader compilation system for AppCore
# Compiles HLSL shaders to platform-specific formats using FXC (Windows), DXC and SPIRV-Cross (macOS/Linux)

# Find required tools
# FXC for Windows D3D11 (Shader Model 4.0)
if(WIN32)
    # Try to find Windows SDK directory
    if(DEFINED ENV{WindowsSdkDir})
        set(WINDOWS_SDK_DIR "$ENV{WindowsSdkDir}")
    elseif(DEFINED ENV{WindowsSDKDir})
        set(WINDOWS_SDK_DIR "$ENV{WindowsSDKDir}")
    else()
        # Try common locations
        file(GLOB WINDOWS_SDK_DIRS 
            "$ENV{ProgramFiles}/Windows Kits/10"
            "$ENV{ProgramFiles}/Windows Kits/8.1"
            "$ENV{ProgramFiles(x86)}/Windows Kits/10"
            "$ENV{ProgramFiles(x86)}/Windows Kits/8.1"
        )
        if(WINDOWS_SDK_DIRS)
            list(GET WINDOWS_SDK_DIRS 0 WINDOWS_SDK_DIR)
        endif()
    endif()
    
    # Find fxc.exe in Windows SDK
    if(WINDOWS_SDK_DIR)
        # Find all version directories and sort them to get the latest
        file(GLOB SDK_BIN_DIRS "${WINDOWS_SDK_DIR}/bin/10.0.*")
        if(SDK_BIN_DIRS)
            list(SORT SDK_BIN_DIRS)
            list(REVERSE SDK_BIN_DIRS)
            foreach(BIN_DIR ${SDK_BIN_DIRS})
                if(EXISTS "${BIN_DIR}/x64/fxc.exe")
                    set(FXC_SEARCH_HINTS "${BIN_DIR}/x64")
                    break()
                endif()
            endforeach()
        endif()
        
        # Also check for 8.1 SDK
        if(NOT FXC_SEARCH_HINTS AND EXISTS "${WINDOWS_SDK_DIR}/bin/x64/fxc.exe")
            set(FXC_SEARCH_HINTS "${WINDOWS_SDK_DIR}/bin/x64")
        endif()
    endif()
    
    find_program(FXC_EXECUTABLE fxc
        HINTS ${FXC_SEARCH_HINTS}
        PATHS "${WINDOWS_SDK_DIR}/bin/x64"
    )
endif()

# DXC for macOS/Linux SPIR-V cross-compilation
find_program(DXC_EXECUTABLE dxc 
    HINTS
    "${CMAKE_SOURCE_DIR}/tools/dxc/bin"
    "$ENV{VULKAN_SDK}/Bin"
    "$ENV{VULKAN_SDK}/bin"
    PATHS
    /usr/bin
    /usr/local/bin
    /opt/homebrew/bin
)

find_program(SPIRV_CROSS_EXECUTABLE spirv-cross
    HINTS
    "${CMAKE_SOURCE_DIR}/tools/spirv-cross/bin"
    PATHS
    /usr/bin
    /usr/local/bin
    /opt/homebrew/bin
)

# Debug mode option
option(APPCORE_SHADER_DEBUG "Enable shader debugging" OFF)

# Main function to compile shaders for the current platform
function(compile_shaders_for_platform TARGET VERTEX_SHADERS PIXEL_SHADERS COMMON_INCLUDES)
    # Create output directory
    set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders/generated)
    file(MAKE_DIRECTORY ${GENERATED_DIR})
    
    # Compile based on platform
    if(WIN32)
        compile_shaders_d3d11(${TARGET} "${VERTEX_SHADERS}" "${PIXEL_SHADERS}" "${COMMON_INCLUDES}" ${GENERATED_DIR})
    elseif(APPLE)
        compile_shaders_metal(${TARGET} "${VERTEX_SHADERS}" "${PIXEL_SHADERS}" "${COMMON_INCLUDES}" ${GENERATED_DIR})
    else()
        compile_shaders_opengl(${TARGET} "${VERTEX_SHADERS}" "${PIXEL_SHADERS}" "${COMMON_INCLUDES}" ${GENERATED_DIR})
    endif()
endfunction()

# D3D11 compilation (Windows)
function(compile_shaders_d3d11 TARGET VERTEX_SHADERS PIXEL_SHADERS COMMON_INCLUDES OUTPUT_DIR)
    if(NOT FXC_EXECUTABLE)
        message(FATAL_ERROR "FXC not found. Please install Windows SDK or ensure fxc.exe is in your PATH.")
    endif()
    
    # Set compilation flags
    if(APPCORE_SHADER_DEBUG)
        set(FXC_FLAGS /Od /Zi)
        message(STATUS "Compiling D3D11 shaders with debug info")
    else()
        set(FXC_FLAGS /O2)
    endif()
    
    set(HEADER_FILES "")
    
    # Compile vertex shaders
    foreach(SHADER_PATH ${VERTEX_SHADERS})
        get_filename_component(SHADER_NAME ${SHADER_PATH} NAME_WE)
        get_filename_component(SHADER_DIR ${SHADER_PATH} DIRECTORY)
        set(OUTPUT_FILE ${OUTPUT_DIR}/${SHADER_NAME}_vs.h)
        set(BYTECODE_FILE ${OUTPUT_DIR}/${SHADER_NAME}_vs.cso)
        
        add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND ${FXC_EXECUTABLE}
                /T vs_4_0
                /E main
                ${FXC_FLAGS}
                /I ${SHADER_DIR}
                /Fo ${BYTECODE_FILE}
                ${SHADER_PATH}
            COMMAND ${CMAKE_COMMAND}
                -DSHADER_NAME=${SHADER_NAME}_vs
                -DBYTECODE_FILE=${BYTECODE_FILE}
                -DOUTPUT_FILE=${OUTPUT_FILE}
                -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedShader.cmake
            DEPENDS 
                ${SHADER_PATH}
                ${COMMON_INCLUDES}
            COMMENT "Compiling D3D11 vertex shader: ${SHADER_NAME}"
            VERBATIM
        )
        list(APPEND HEADER_FILES ${OUTPUT_FILE})
    endforeach()
    
    # Compile pixel shaders
    foreach(SHADER_PATH ${PIXEL_SHADERS})
        get_filename_component(SHADER_NAME ${SHADER_PATH} NAME_WE)
        get_filename_component(SHADER_DIR ${SHADER_PATH} DIRECTORY)
        set(OUTPUT_FILE ${OUTPUT_DIR}/${SHADER_NAME}_ps.h)
        set(BYTECODE_FILE ${OUTPUT_DIR}/${SHADER_NAME}_ps.cso)
        
        add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND ${FXC_EXECUTABLE}
                /T ps_4_0
                /E main
                ${FXC_FLAGS}
                /I ${SHADER_DIR}
                /Fo ${BYTECODE_FILE}
                ${SHADER_PATH}
            COMMAND ${CMAKE_COMMAND}
                -DSHADER_NAME=${SHADER_NAME}_ps
                -DBYTECODE_FILE=${BYTECODE_FILE}
                -DOUTPUT_FILE=${OUTPUT_FILE}
                -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedShader.cmake
            DEPENDS 
                ${SHADER_PATH}
                ${COMMON_INCLUDES}
            COMMENT "Compiling D3D11 pixel shader: ${SHADER_NAME}"
            VERBATIM
        )
        list(APPEND HEADER_FILES ${OUTPUT_FILE})
    endforeach()
    
    # Add include directory for generated headers
    target_include_directories(${TARGET} PRIVATE ${OUTPUT_DIR})
    
    # Set explicit object dependencies for the source file that includes the shader headers
    # This ensures proper dependency tracking with Ninja and other build systems
    set_source_files_properties(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/win/d3d11/GPUDriverD3D11.cpp
        PROPERTIES OBJECT_DEPENDS "${HEADER_FILES}"
    )
    
    # Create a custom target to ensure shader generation happens before compilation
    add_custom_target(${TARGET}_d3d11_shaders ALL DEPENDS ${HEADER_FILES})
    add_dependencies(${TARGET} ${TARGET}_d3d11_shaders)
endfunction()

# Metal compilation (macOS)
function(compile_shaders_metal TARGET VERTEX_SHADERS PIXEL_SHADERS COMMON_INCLUDES OUTPUT_DIR)
    if(NOT DXC_EXECUTABLE OR NOT SPIRV_CROSS_EXECUTABLE)
        message(FATAL_ERROR "DXC and SPIRV-Cross are required for Metal shader compilation.")
    endif()
    
    set(HEADER_FILES "")
    
    # Process all shaders
    foreach(SHADER_FILE ${VERTEX_SHADERS} ${PIXEL_SHADERS})
        get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
        
        # Determine shader type
        if(${SHADER_FILE} IN_LIST VERTEX_SHADERS)
            set(SHADER_PROFILE "vs_5_0")
            set(SHADER_SUFFIX "_vs")
        else()
            set(SHADER_PROFILE "ps_5_0") 
            set(SHADER_SUFFIX "_ps")
        endif()
        
        set(SPIRV_FILE ${OUTPUT_DIR}/${SHADER_NAME}.spv)
        set(MSL_FILE ${OUTPUT_DIR}/${SHADER_NAME}.metal)
        set(AIR_FILE ${OUTPUT_DIR}/${SHADER_NAME}.air)
        set(METALLIB_FILE ${OUTPUT_DIR}/${SHADER_NAME}${SHADER_SUFFIX}.metallib)
        set(HEADER_FILE ${OUTPUT_DIR}/${SHADER_NAME}${SHADER_SUFFIX}.h)
        
        # HLSL → SPIR-V
        add_custom_command(
            OUTPUT ${SPIRV_FILE}
            COMMAND ${DXC_EXECUTABLE}
                -T ${SHADER_PROFILE}
                -E main
                -spirv
                -fspv-target-env=vulkan1.1
                -O3
                -I ${CMAKE_CURRENT_SOURCE_DIR}/shaders/hlsl
                -Fo ${SPIRV_FILE}
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders/hlsl/${SHADER_FILE}
            DEPENDS 
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders/hlsl/${SHADER_FILE}
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders/hlsl/common.hlsli
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders/hlsl/clip.hlsli
            COMMENT "Compiling ${SHADER_FILE} to SPIR-V"
            VERBATIM
        )
        
        # SPIR-V → Metal
        add_custom_command(
            OUTPUT ${MSL_FILE}
            COMMAND ${SPIRV_CROSS_EXECUTABLE}
                ${SPIRV_FILE}
                --msl
                --msl-version 20100
                --msl-argument-buffers
                --output ${MSL_FILE}
            DEPENDS ${SPIRV_FILE}
            COMMENT "Cross-compiling ${SHADER_NAME} to Metal"
            VERBATIM
        )
        
        # Only compile Metal on macOS
        if(APPLE)
            # Set Metal compilation flags
            if(APPCORE_SHADER_DEBUG)
                set(METAL_FLAGS -O0 -gline-tables-only -frecord-sources)
            else()
                set(METAL_FLAGS -O3)
            endif()
            
            # Metal → AIR
            add_custom_command(
                OUTPUT ${AIR_FILE}
                COMMAND xcrun -sdk macosx metal
                    -c ${MSL_FILE}
                    -o ${AIR_FILE}
                    -std=osx-metal2.0
                    ${METAL_FLAGS}
                DEPENDS ${MSL_FILE}
                COMMENT "Compiling ${SHADER_NAME} Metal to AIR"
                VERBATIM
            )
            
            # AIR → Metallib
            add_custom_command(
                OUTPUT ${METALLIB_FILE}
                COMMAND xcrun -sdk macosx metallib
                    ${AIR_FILE}
                    -o ${METALLIB_FILE}
                DEPENDS ${AIR_FILE}
                COMMENT "Creating Metal library for ${SHADER_NAME}"
                VERBATIM
            )
            
            # Metallib → Header
            add_custom_command(
                OUTPUT ${HEADER_FILE}
                COMMAND ${CMAKE_COMMAND}
                    -DSHADER_NAME=${SHADER_NAME}${SHADER_SUFFIX}
                    -DMETALLIB_FILE=${METALLIB_FILE}
                    -DOUTPUT_FILE=${HEADER_FILE}
                    -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedMetal.cmake
                DEPENDS ${METALLIB_FILE}
                COMMENT "Embedding Metal shader: ${SHADER_NAME}"
                VERBATIM
            )
        else()
            # For non-Apple platforms, just create a dummy file
            add_custom_command(
                OUTPUT ${HEADER_FILE}
                COMMAND ${CMAKE_COMMAND} -E touch ${HEADER_FILE}
                DEPENDS ${MSL_FILE}
                COMMENT "Creating placeholder for ${SHADER_NAME} on non-Apple platform"
            )
        endif()
        
        list(APPEND HEADER_FILES ${HEADER_FILE})
    endforeach()
    
    # Add headers to target
    target_sources(${TARGET} PRIVATE ${HEADER_FILES})
    
    # Add include directory for generated headers
    target_include_directories(${TARGET} PRIVATE ${OUTPUT_DIR})
endfunction()

# OpenGL compilation (Linux)
function(compile_shaders_opengl TARGET VERTEX_SHADERS PIXEL_SHADERS COMMON_INCLUDES OUTPUT_DIR)
    if(NOT DXC_EXECUTABLE OR NOT SPIRV_CROSS_EXECUTABLE)
        message(FATAL_ERROR "DXC and SPIRV-Cross are required for OpenGL shader compilation.")
    endif()
    
    set(HEADER_FILES "")
    
    foreach(SHADER_FILE ${VERTEX_SHADERS} ${PIXEL_SHADERS})
        get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
        
        # Determine shader type
        if(${SHADER_FILE} IN_LIST VERTEX_SHADERS)
            set(SHADER_PROFILE "vs_5_0")
            set(SHADER_SUFFIX "_vs")
        else()
            set(SHADER_PROFILE "ps_5_0")
            set(SHADER_SUFFIX "_fs")
        endif()
        
        set(SPIRV_FILE ${OUTPUT_DIR}/${SHADER_NAME}.spv)
        set(GLSL_FILE ${OUTPUT_DIR}/${SHADER_NAME}.glsl)
        set(HEADER_FILE ${OUTPUT_DIR}/${SHADER_NAME}${SHADER_SUFFIX}.h)
        
        # HLSL → SPIR-V
        add_custom_command(
            OUTPUT ${SPIRV_FILE}
            COMMAND ${DXC_EXECUTABLE}
                -T ${SHADER_PROFILE}
                -E main
                -spirv
                -fspv-target-env=vulkan1.1
                -O3
                -I ${CMAKE_CURRENT_SOURCE_DIR}/shaders/hlsl
                -Fo ${SPIRV_FILE}
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders/hlsl/${SHADER_FILE}
            DEPENDS 
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders/hlsl/${SHADER_FILE}
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders/hlsl/common.hlsli
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders/hlsl/clip.hlsli
            COMMENT "Compiling ${SHADER_FILE} to SPIR-V for OpenGL"
            VERBATIM
        )
        
        # SPIR-V → GLSL
        add_custom_command(
            OUTPUT ${GLSL_FILE}
            COMMAND ${SPIRV_CROSS_EXECUTABLE}
                ${SPIRV_FILE}
                --version 330
                --es
                --no-420pack-extension
                --output ${GLSL_FILE}
            DEPENDS ${SPIRV_FILE}
            COMMENT "Cross-compiling ${SHADER_NAME} to GLSL"
            VERBATIM
        )
        
        # Embed GLSL source
        add_custom_command(
            OUTPUT ${HEADER_FILE}
            COMMAND ${CMAKE_COMMAND}
                -DSHADER_NAME=${SHADER_NAME}${SHADER_SUFFIX}
                -DGLSL_FILE=${GLSL_FILE}
                -DOUTPUT_FILE=${HEADER_FILE}
                -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedGLSL.cmake
            DEPENDS ${GLSL_FILE}
            COMMENT "Embedding GLSL source: ${SHADER_NAME}"
            VERBATIM
        )
        
        list(APPEND HEADER_FILES ${HEADER_FILE})
    endforeach()
    
    # Add headers to target
    target_sources(${TARGET} PRIVATE ${HEADER_FILES})
    
    # Add include directory for generated headers
    target_include_directories(${TARGET} PRIVATE ${OUTPUT_DIR})
endfunction()