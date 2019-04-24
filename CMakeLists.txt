cmake_minimum_required(VERSION 3.3.2)
project(AppCore C CXX)
set_property(DIRECTORY . PROPERTY FOLDER "AppCore")

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(AppCore)

add_definitions(-DAPPCORE_IMPLEMENTATION)

if (UNIX)
    if (APPLE)
		set(PORT UltralightMac)
    else ()
		set(PORT UltralightLinux)
    endif ()
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(PORT UltralightWin)
else ()
    message(FATAL_ERROR "Unknown OS '${CMAKE_SYSTEM_NAME}'")
endif ()

include(Deps.cmake)

include_directories(
    "include"
    "${ULTRALIGHTCORE_DIR}/include"
    "${WEBCORE_DIR}/include"
    "${ULTRALIGHT_DIR}/include"
    "src/common"
    )

load_sources(AppCore_SOURCES "include")
load_sources(AppCore_SOURCES "src/common")
    
if (PORT MATCHES "UltralightLinux")
    link_directories("${ULTRALIGHTCORE_DIR}/bin"
                     "${WEBCORE_DIR}/bin"
                     "${ULTRALIGHT_DIR}/bin")

    if (${BUILD_DBG})
        # Debug Config
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DNDEBUG -ffunction-sections -fdata-sections -fPIC -std=c++14 -Wno-trigraphs -fno-exceptions -Wno-missing-field-initializers -Wnon-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wsign-compare -Wno-shorten-64-to-32 -Wno-c++11-extensions -DU_DISABLE_RENAMING=1 -DU_SHOW_CPLUSPLUS_API=0 -DU_STATIC_IMPLEMENTATION=1 -Wdeprecated-declarations -Winvalid-offsetof -fvisibility=hidden -fno-threadsafe-statics -Wno-sign-conversion -Winfinite-recursion -Wno-strict-prototypes")
    else ()
        # Release Config
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=nehalem -O3 -DNDEBUG -ffunction-sections -fdata-sections -fPIC -std=c++14 -Wno-trigraphs -fno-exceptions -Wno-missing-field-initializers -Wnon-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wsign-compare -Wno-shorten-64-to-32 -Wno-c++11-extensions -DU_DISABLE_RENAMING=1 -DU_SHOW_CPLUSPLUS_API=0 -DU_STATIC_IMPLEMENTATION=1 -Wdeprecated-declarations -Winvalid-offsetof -fvisibility=hidden -fno-threadsafe-statics -Wno-sign-conversion -Winfinite-recursion -Wno-strict-prototypes")
    endif ()

    # Disable various GLFW options
    option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
    option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
    option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
    option(GLFW_INSTALL "Generate installation target" OFF)

    set(GLFW_DIR "src/glfw")
    add_subdirectory(${GLFW_DIR})

    load_sources(AppCore_SOURCES "src/linux")

    list(APPEND AppCore_SOURCES
        "${GLFW_DIR}/deps/glad/glad.h"
        "${GLFW_DIR}/deps/glad.c"
    )
                 
    include_directories("${GLFW_DIR}/include")
    include_directories("${GLFW_DIR}/deps")
    
    link_libraries(glfw)
elseif (PORT MATCHES "UltralightMac")
    link_directories("${ULTRALIGHTCORE_DIR}/bin"
                     "${WEBCORE_DIR}/bin"
                     "${ULTRALIGHT_DIR}/bin")

    if (${BUILD_DBG})
        # Debug Config
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g -DNDEBUG -std=gnu++14 -stdlib=libc++ -Wno-trigraphs -fno-exceptions -Wno-missing-field-initializers -Wnon-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wsign-compare -Wno-shorten-64-to-32 -Wno-c++11-extensions -DU_DISABLE_RENAMING=1 -DU_SHOW_CPLUSPLUS_API=0 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk -Wdeprecated-declarations -Winvalid-offsetof -fvisibility=hidden -fno-threadsafe-statics -Wno-sign-conversion -Winfinite-recursion -Wmove -Wno-comma -Wblock-capture-autoreleasing -Wno-strict-prototypes -Wno-nonportable-include-path -frtti")
    else ()
        # Release Configs
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -march=nehalem -O3 -DNDEBUG -std=gnu++14 -stdlib=libc++ -Wno-trigraphs -fno-exceptions -Wno-missing-field-initializers -Wnon-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wsign-compare -Wno-shorten-64-to-32 -Wno-c++11-extensions -DU_DISABLE_RENAMING=1 -DU_SHOW_CPLUSPLUS_API=0 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk -Wdeprecated-declarations -Winvalid-offsetof -fvisibility=hidden -fno-threadsafe-statics -Wno-sign-conversion -Winfinite-recursion -Wmove -Wno-comma -Wblock-capture-autoreleasing -Wno-strict-prototypes -Wno-nonportable-include-path -frtti")
    endif ()

    load_sources(AppCore_SOURCES "src/mac")

    list(APPEND AppCore_SOURCES
#        "shaders/metal/Shaders.metal"
    )

    set_source_files_properties("shaders/metal/Shaders.metal" PROPERTIES LANGUAGE METAL)
elseif (PORT MATCHES "UltralightWin")
    link_directories("${ULTRALIGHTCORE_DIR}/lib"
                     "${WEBCORE_DIR}/lib"
                     "${ULTRALIGHT_DIR}/lib")

    load_sources(AppCore_SOURCES "src/win")
    
    add_definitions(-D_UNICODE -DNOMINMAX)
    link_libraries("Shlwapi.lib" "Usp10.lib" "d3d11.lib" "d3dcompiler.lib")
endif ()

link_libraries(UltralightCore WebCore Ultralight)

add_library(AppCore SHARED ${AppCore_SOURCES})

add_dependencies(AppCore UltralightCoreBin WebCoreBin UltralightBin)

if (PORT MATCHES "UltralightMac")
    set_target_properties(AppCore PROPERTIES LINK_FLAGS "-framework Cocoa -framework Metal -framework Foundation -framework CoreFoundation -framework MetalKit")
    set_property (TARGET AppCore APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")
endif ()

include(CreateSDK.cmake)