# CompileMetalShaders.cmake
# macOS-only build step to compile pre-generated Metal source files to metallib bytecode

# Function to compile Metal source to metallib and generate binary header
function(compile_metal_shader_to_header METAL_SOURCE_FILE OUTPUT_HEADER_FILE VARIABLE_NAME)
    get_filename_component(SHADER_NAME ${METAL_SOURCE_FILE} NAME_WE)
    
    # Intermediate files
    set(AIR_FILE ${CMAKE_CURRENT_BINARY_DIR}/shaders/metal/${SHADER_NAME}.air)
    set(METALLIB_FILE ${CMAKE_CURRENT_BINARY_DIR}/shaders/metal/${SHADER_NAME}.metallib)
    
    # Create Metal build directory
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders/metal)
    
    # Debug/Release flags
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        set(METAL_FLAGS -O0 -gline-tables-only -frecord-sources)
    else()
        set(METAL_FLAGS -O3)
    endif()
    
    # Metal source → AIR
    add_custom_command(
        OUTPUT ${AIR_FILE}
        COMMAND xcrun -sdk macosx metal
            -c ${METAL_SOURCE_FILE}
            -o ${AIR_FILE}
            -std=osx-metal2.1
            ${METAL_FLAGS}
        DEPENDS ${METAL_SOURCE_FILE}
        COMMENT "Compiling Metal shader: ${SHADER_NAME}.metal → ${SHADER_NAME}.air"
        VERBATIM
    )
    
    # AIR → Metallib
    add_custom_command(
        OUTPUT ${METALLIB_FILE}
        COMMAND xcrun -sdk macosx metallib
            ${AIR_FILE}
            -o ${METALLIB_FILE}
        DEPENDS ${AIR_FILE}
        COMMENT "Creating Metal library: ${SHADER_NAME}.air → ${SHADER_NAME}.metallib"
        VERBATIM
    )
    
    # Metallib → Binary Header
    add_custom_command(
        OUTPUT ${OUTPUT_HEADER_FILE}
        COMMAND ${CMAKE_COMMAND}
            -DSHADER_NAME=${VARIABLE_NAME}
            -DMETALLIB_FILE=${METALLIB_FILE}
            -DOUTPUT_FILE=${OUTPUT_HEADER_FILE}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedMetalBinary.cmake
        DEPENDS ${METALLIB_FILE}
        COMMENT "Embedding Metal shader: ${SHADER_NAME}.metallib → ${VARIABLE_NAME}.h"
        VERBATIM
    )
endfunction()

# Function to compile all Metal shaders if on macOS
function(compile_metal_shaders_if_needed TARGET)
    if(NOT APPLE)
        return()
    endif()
    
    # Check if pre-generated Metal source files exist
    set(METAL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders/generated/metal)
    if(NOT EXISTS ${METAL_SOURCE_DIR})
        message(WARNING "Metal source directory not found: ${METAL_SOURCE_DIR}")
        message(WARNING "Run shaders/build-shaders/build-shaders.ps1 on Windows to generate Metal source files")
        return()
    endif()
    
    # Find all .metal files in generated directory
    file(GLOB METAL_SOURCE_FILES "${METAL_SOURCE_DIR}/*.metal")
    if(NOT METAL_SOURCE_FILES)
        message(WARNING "No Metal source files found in: ${METAL_SOURCE_DIR}")
        message(WARNING "Run shaders/build-shaders/build-shaders.ps1 on Windows to generate Metal source files")
        return()
    endif()
    
    set(METAL_HEADER_FILES "")
    
    # Compile each Metal source file
    foreach(METAL_FILE ${METAL_SOURCE_FILES})
        get_filename_component(SHADER_NAME ${METAL_FILE} NAME_WE)
        
        # Determine suffix and variable name
        if(${SHADER_NAME} MATCHES "vertex_")
            set(SUFFIX "_vs")
        else()
            set(SUFFIX "_ps")
        endif()
        
        set(VARIABLE_NAME "${SHADER_NAME}${SUFFIX}")
        set(HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/shaders/metal/${VARIABLE_NAME}.h)
        
        # Compile this Metal shader
        compile_metal_shader_to_header(${METAL_FILE} ${HEADER_FILE} ${VARIABLE_NAME})
        
        list(APPEND METAL_HEADER_FILES ${HEADER_FILE})
    endforeach()
    
    # Generate Metal master header
    set(METAL_MASTER_HEADER ${CMAKE_CURRENT_BINARY_DIR}/shaders/metal/shaders.h)
    add_custom_command(
        OUTPUT ${METAL_MASTER_HEADER}
        COMMAND ${CMAKE_COMMAND}
            -DMETAL_HEADER_FILES="${METAL_HEADER_FILES}"
            -DOUTPUT_FILE=${METAL_MASTER_HEADER}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateMetalMasterHeader.cmake
        DEPENDS ${METAL_HEADER_FILES}
        COMMENT "Generating Metal master header"
        VERBATIM
    )
    
    # Create custom target for Metal compilation
    add_custom_target(${TARGET}_metal_shaders
        DEPENDS ${METAL_HEADER_FILES} ${METAL_MASTER_HEADER}
        COMMENT "Compiling Metal shaders to metallib bytecode"
    )
    
    # Add dependency to main target
    add_dependencies(${TARGET} ${TARGET}_metal_shaders)
    
    # Add Metal headers to include path
    target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/shaders/metal)
    
    message(STATUS "Metal shader compilation enabled for ${TARGET}")
    message(STATUS "  Source directory: ${METAL_SOURCE_DIR}")
    list(LENGTH METAL_SOURCE_FILES METAL_FILE_COUNT)
    message(STATUS "  Found ${METAL_FILE_COUNT} Metal source files")
endfunction()