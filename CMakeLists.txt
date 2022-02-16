cmake_minimum_required(VERSION 3.15.0)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(common)

project(AppCore C CXX)
set_property(DIRECTORY . PROPERTY FOLDER "AppCore")

include(ExternalProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DAPPCORE_IMPLEMENTATION)

# This should be set in platform toolchain.
if (NOT PORT)
    message(FATAL_ERROR "Unknown PORT '${PORT}'")
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

    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        # Debug Config
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DNDEBUG -ffunction-sections -fdata-sections -fPIC -std=c++14 -Wno-trigraphs -fno-exceptions -Wno-missing-field-initializers -Wnon-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wsign-compare -Wno-shorten-64-to-32 -Wno-c++11-extensions -DU_DISABLE_RENAMING=1 -DU_SHOW_CPLUSPLUS_API=0 -DU_STATIC_IMPLEMENTATION=1 -Wdeprecated-declarations -Winvalid-offsetof -fvisibility=hidden -fno-threadsafe-statics -Wno-sign-conversion -Winfinite-recursion -Wno-strict-prototypes")
    else ()
        # Release Config
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=core2 -mtune=haswell -Oz -DNDEBUG -ffunction-sections -fdata-sections -fPIC -std=c++14 -Wno-trigraphs -fno-exceptions -Wno-missing-field-initializers -Wnon-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wsign-compare -Wno-shorten-64-to-32 -Wno-c++11-extensions -DU_DISABLE_RENAMING=1 -DU_SHOW_CPLUSPLUS_API=0 -DU_STATIC_IMPLEMENTATION=1 -Wdeprecated-declarations -Winvalid-offsetof -fvisibility=hidden -fno-threadsafe-statics -Wno-sign-conversion -Winfinite-recursion -Wno-strict-prototypes")
    endif ()
    

    # Allow relocatable binaries on Linux, load from executable path
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath='$ORIGIN'")


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
    
    link_libraries(glfw fontconfig)
elseif (PORT MATCHES "UltralightMac")
    link_directories("${ULTRALIGHTCORE_DIR}/bin"
                     "${WEBCORE_DIR}/bin"
                     "${ULTRALIGHT_DIR}/bin")

    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        # Debug Config
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g -DNDEBUG -fPIC -std=gnu++14 -stdlib=libc++ -Wno-trigraphs -fno-exceptions -Wno-missing-field-initializers -Wnon-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wsign-compare -Wno-shorten-64-to-32 -Wno-c++11-extensions -DU_DISABLE_RENAMING=1 -DU_SHOW_CPLUSPLUS_API=0 -isysroot ${CMAKE_SYSROOT} -mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET} -Wdeprecated-declarations -Winvalid-offsetof -fvisibility=hidden -fno-threadsafe-statics -Wno-sign-conversion -Winfinite-recursion -Wmove -Wno-comma -Wblock-capture-autoreleasing -Wno-strict-prototypes -Wno-nonportable-include-path -frtti")
    else ()
        # Release Configs
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -march=core2 -mtune=haswell -Oz -DNDEBUG -fPIC -std=gnu++14 -stdlib=libc++ -Wno-trigraphs -fno-exceptions -Wno-missing-field-initializers -Wnon-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wsign-compare -Wno-shorten-64-to-32 -Wno-c++11-extensions -DU_DISABLE_RENAMING=1 -DU_SHOW_CPLUSPLUS_API=0 -isysroot ${CMAKE_SYSROOT} -mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET} -Wdeprecated-declarations -Winvalid-offsetof -fvisibility=hidden -fno-threadsafe-statics -Wno-sign-conversion -Winfinite-recursion -Wmove -Wno-comma -Wblock-capture-autoreleasing -Wno-strict-prototypes -Wno-nonportable-include-path -frtti")
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
    
    if (UL_D3D_DRIVER STREQUAL "d3d11")
        add_definitions(-DDRIVER_D3D11)
        link_libraries("d3d11.lib" "d3dcompiler.lib")
    elseif (UL_D3D_DRIVER STREQUAL "d3d12")
        add_definitions(-DDRIVER_D3D12)
        link_libraries("d3d12.lib" "dxgi.lib" "d3dcompiler.lib")
    else ()
        message(FATAL_ERROR "Unknown UL_D3D_DRIVER '${UL_D3D_DRIVER}'")
    endif ()
    
    add_definitions(-D_UNICODE -DUNICODE -DNOMINMAX)
    link_libraries("Shlwapi.lib" "Usp10.lib" "Dwrite.lib")

    foreach (flag_var
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        # Reset all project configurations to use /MD so we can override below
        string(REGEX REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
        string(REGEX REPLACE "/MDd" "/MD" ${flag_var} "${${flag_var}}")
        string(REGEX REPLACE "/D_DEBUG" "" ${flag_var} "${${flag_var}}")

        if (CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreaded")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        elseif (CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreadedDLL")
            # no-op, already /MD
        elseif(CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreadedDebug")
            string(REGEX REPLACE "/MD" "/MTd" ${flag_var} "${${flag_var}}")
        elseif(CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreadedDebugDLL")
            string(REGEX REPLACE "/MD" "/MDd" ${flag_var} "${${flag_var}}")
        else()
            message(FATAL_ERROR "Unknown MSVC runtime library: '${CMAKE_MSVC_RUNTIME_LIBRARY}'")
        endif()
    endforeach ()

    if (CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreaded")
        message(STATUS "Building projects with MultiThreaded Static Runtime (/MT)")
    elseif (CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreadedDLL")
        message(STATUS "Building projects with MultiThreaded DLL Runtime (/MD)")
    elseif(CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreadedDebug")
        message(STATUS "Building projects with MultiThreaded Static Debug Runtime (/MTd)")
    elseif(CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreadedDebugDLL")
        message(STATUS "Building projects with MultiThreaded DLL Debug Runtime (/MDd)")
    else()
        message(FATAL_ERROR "Unknown MSVC runtime library: '${CMAKE_MSVC_RUNTIME_LIBRARY}'")
    endif()
endif ()

if (UL_PROFILE_PERFORMANCE OR UL_PROFILE_MEMORY)
    add_definitions(-DTRACY_ENABLE)
    add_definitions(-DTRACY_IMPORTS)
    add_definitions(-DTRACY_MODULE_APPCORE)
    add_definitions(-DTRACY_CALLSTACK_DEPTH=${UL_CALLSTACK_DEPTH})
endif ()

if (UL_PROFILE_PERFORMANCE)
    add_definitions(-DTRACY_PROFILE_PERFORMANCE)
endif ()

if (UL_PROFILE_MEMORY)
    add_definitions(-DTRACY_PROFILE_MEMORY)
endif ()

link_libraries(UltralightCore WebCore Ultralight)

if (UL_ENABLE_STATIC_BUILD)
    add_definitions(-DULTRALIGHT_STATIC_BUILD)
    add_library(AppCore STATIC ${AppCore_SOURCES})
else ()
    add_library(AppCore SHARED ${AppCore_SOURCES})
endif ()

add_dependencies(AppCore UltralightCoreBin WebCoreBin UltralightBin)

if (PORT MATCHES "UltralightMac")
    set_target_properties(AppCore PROPERTIES LINK_FLAGS "-framework Cocoa -framework Metal -framework Foundation -framework CoreFoundation -framework QuartzCore -framework MetalKit")
    set_property (TARGET AppCore APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")
endif ()

if(UL_ENABLE_TESTS)
    if (PORT MATCHES "UltralightLinux")
        SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
        SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        SET(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")
    endif ()

    if (PORT MATCHES "UltralightMac")
        SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
        SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        SET(CMAKE_INSTALL_RPATH "@executable_path/")
    endif ()

    add_executable(MiniBrowser MACOSX_BUNDLE "tests/MiniBrowser/MiniBrowser.cpp" ${AppCore_SOURCES})
    
    add_dependencies(MiniBrowser UltralightCoreBin WebCoreBin UltralightBin)

    if (UL_ENABLE_STATIC_BUILD)
        if (NOT ALLINONE_BUILD)
            message(FATAL_ERROR "Static build of MiniBrowser is only supported in all-in-one builds.")
        endif ()

        target_link_libraries(MiniBrowser
            WTF
            JavaScriptCore
            PAL
            skia
            mimalloc
            icuin
            icuuc
            icudt
            brotlicommon
            brotlidec
            brotlienc
            crypto
            freetype
            harfbuzz
            harfbuzz-icu
            jpeg-static
            libcurl
            libpng16_static
            nghttp2_static
            sqlite3
            ssl
            tls
            xml2
            xslt
            zlibstatic
        )

        if (UL_ENABLE_VIDEO)
            target_link_libraries(MiniBrowser
                gstreamer-full-1.0
                gio-2.0
                glib-2.0
                gmodule-2.0
                gobject-2.0)
        endif ()

        if (PORT MATCHES "UltralightWin")
            target_link_libraries(MiniBrowser
                comctl32
                crypt32
                iphlpapi
                normaliz
                rpcrt4
                shlwapi
                usp10
                version
                winmm
                wldap32
                ws2_32
                bcrypt
            )
        endif ()
    endif ()

    if (APPLE)
      # Enable High-DPI on macOS through our custom Info.plist template
      set_target_properties(MiniBrowser PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/tests/MiniBrowser/Info.plist.in") 
      set_target_properties(MiniBrowser PROPERTIES LINK_FLAGS "-framework Cocoa -framework Metal -framework Foundation -framework CoreFoundation -framework QuartzCore -framework MetalKit")
      set_property (TARGET MiniBrowser APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")
    endif()

    if (MSVC)
      # Tell MSVC to use main instead of WinMain for Windows subsystem executables
      set_target_properties(MiniBrowser PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
      
      if (NOT ${ALLINONE_BUILD})
        add_custom_command(TARGET MiniBrowser POST_BUILD
            COMMAND xcopy /E /Q /D /Y $<1:"${ULTRALIGHTCORE_DIR}/bin"> $<1:"$<TARGET_FILE_DIR:MiniBrowser>">
            COMMAND xcopy /E /Q /D /Y $<1:"${WEBCORE_DIR}/bin"> $<1:"$<TARGET_FILE_DIR:MiniBrowser>">
            COMMAND xcopy /E /Q /D /Y $<1:"${ULTRALIGHT_DIR}/bin"> $<1:"$<TARGET_FILE_DIR:MiniBrowser>">
        )
      endif()
    endif()

    if (APPLE)
      set(RESOURCES_PATH "$<TARGET_FILE_DIR:MiniBrowser>/../Resources") 
    else ()
      set(RESOURCES_PATH "$<TARGET_FILE_DIR:MiniBrowser>") 
    endif ()

    # Copy assets to assets path
    add_custom_command(TARGET MiniBrowser POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/tests/MiniBrowser/assets/" "${RESOURCES_PATH}/assets/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${WEBCORE_DIR}/bin/resources/" "${RESOURCES_PATH}/assets/resources/")

    if (APPLE)
        if (NOT ${ALLINONE_BUILD})
            add_custom_command(TARGET MiniBrowser POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHTCORE_DIR}/bin/libUltralightCore.dylib" $<TARGET_FILE_DIR:MiniBrowser>
                COMMAND ${CMAKE_COMMAND} -E copy "${WEBCORE_DIR}/bin/*.dylib" $<TARGET_FILE_DIR:MiniBrowser>
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHT_DIR}/bin/libUltralight.dylib" $<TARGET_FILE_DIR:MiniBrowser>)
        endif()
    endif ()

    if (PORT MATCHES "UltralightLinux")
        set_target_properties(MiniBrowser PROPERTIES LINK_FLAGS "-Wl,-rpath-link,${WEBCORE_DIR}/bin/")

        if (NOT ${ALLINONE_BUILD})
            add_custom_command(TARGET MiniBrowser POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHTCORE_DIR}/bin/libUltralightCore.so" $<TARGET_FILE_DIR:MiniBrowser>
                COMMAND ${CMAKE_COMMAND} -E copy "${WEBCORE_DIR}/bin/lib*" $<TARGET_FILE_DIR:MiniBrowser>
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHT_DIR}/bin/libUltralight.so" $<TARGET_FILE_DIR:MiniBrowser>)
        endif()
    endif ()

    if (${ALLINONE_BUILD})
        add_custom_command(TARGET MiniBrowser POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:MiniBrowser>" ${CMAKE_INSTALL_PREFIX}/bin)
        add_custom_command(TARGET MiniBrowser POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/tests/MiniBrowser/assets/" "${CMAKE_INSTALL_PREFIX}/bin/assets"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${WEBCORE_DIR}/bin/resources/" "${CMAKE_INSTALL_PREFIX}/bin/assets/resources/")
    endif()
endif ()

include(CreateSDK.cmake)
