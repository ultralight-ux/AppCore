macro(CreateTestApp APP_NAME)

    load_sources(${APP_NAME}_SOURCES "tests/${APP_NAME}")
    add_executable(${APP_NAME} MACOSX_BUNDLE ${${APP_NAME}_SOURCES} ${AppCore_SOURCES})
    set_target_properties(${APP_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${APP_NAME}"
    )
    
    add_dependencies(${APP_NAME} UltralightCoreBin WebCoreBin UltralightBin)

    if (UL_ENABLE_STATIC_BUILD)
        if (NOT ALLINONE_BUILD)
            message(FATAL_ERROR "Static build of ${APP_NAME} is only supported in all-in-one builds.")
        endif ()

        target_link_libraries(${APP_NAME}
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
            target_link_libraries(${APP_NAME}
                gstreamer-full-1.0
                gio-2.0
                glib-2.0
                gmodule-2.0
                gobject-2.0)
        endif ()

        if (PORT MATCHES "UltralightWin")
            target_link_libraries(${APP_NAME}
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
      set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/tests/${APP_NAME}/Info.plist.in") 
      set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "-framework Cocoa -framework Metal -framework Foundation -framework CoreFoundation -framework QuartzCore -framework MetalKit")
      set_property (TARGET ${APP_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")
    endif()

    if (MSVC)
        # Tell MSVC to use main instead of WinMain for Windows subsystem executables
        set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
      
        if (NOT ${ALLINONE_BUILD})
            add_custom_command(TARGET ${APP_NAME} POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHTCORE_DIR}/bin/" $<TARGET_FILE_DIR:${APP_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${WEBCORE_DIR}/bin/" $<TARGET_FILE_DIR:${APP_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_DIR}/bin/" $<TARGET_FILE_DIR:${APP_NAME}>)
        endif()
    endif()
    

    if (APPLE)
      set(RESOURCES_PATH "$<TARGET_FILE_DIR:${APP_NAME}>/../Resources") 
    else ()
      set(RESOURCES_PATH "$<TARGET_FILE_DIR:${APP_NAME}>") 
    endif ()

    # Copy assets to assets path
    add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/tests/${APP_NAME}/assets/" "${RESOURCES_PATH}/assets/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${WEBCORE_DIR}/bin/resources/" "${RESOURCES_PATH}/assets/resources/")

    if (APPLE)
        if (NOT ${ALLINONE_BUILD})
            add_custom_command(TARGET ${APP_NAME} POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHTCORE_DIR}/bin/libUltralightCore.dylib" $<TARGET_FILE_DIR:${APP_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy "${WEBCORE_DIR}/bin/*.dylib" $<TARGET_FILE_DIR:${APP_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHT_DIR}/bin/libUltralight.dylib" $<TARGET_FILE_DIR:${APP_NAME}>)
        endif()
    endif ()

    if (PORT MATCHES "UltralightLinux")
        set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath-link,${WEBCORE_DIR}/bin/")

        if (NOT ${ALLINONE_BUILD})
            add_custom_command(TARGET ${APP_NAME} POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHTCORE_DIR}/bin/libUltralightCore.so" $<TARGET_FILE_DIR:${APP_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy "${WEBCORE_DIR}/bin/lib*" $<TARGET_FILE_DIR:${APP_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHT_DIR}/bin/libUltralight.so" $<TARGET_FILE_DIR:${APP_NAME}>)
        endif()
    endif ()

    if (${ALLINONE_BUILD})
        add_custom_command(TARGET ${APP_NAME} POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${APP_NAME}>" ${CMAKE_INSTALL_PREFIX}/bin)
        add_custom_command(TARGET ${APP_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/tests/${APP_NAME}/assets/" "${CMAKE_INSTALL_PREFIX}/bin/assets"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${WEBCORE_DIR}/bin/resources/" "${CMAKE_INSTALL_PREFIX}/bin/assets/resources/")
    endif()

endmacro()