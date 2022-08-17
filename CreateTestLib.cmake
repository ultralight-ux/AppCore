macro(CreateTestLib LIB_NAME)

    load_sources(${LIB_NAME}_SOURCES "tests/${LIB_NAME}")
    add_library(${LIB_NAME} SHARED ${${LIB_NAME}_SOURCES} ${AppCore_SOURCES})
    set_target_properties(${LIB_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${LIB_NAME}"
    )
    
    add_dependencies(${LIB_NAME} UltralightCoreBin WebCoreBin UltralightBin)

    if (UL_ENABLE_STATIC_BUILD)
        if (NOT ALLINONE_BUILD)
            message(FATAL_ERROR "Static build of ${LIB_NAME} is only supported in all-in-one builds.")
        endif ()

        target_link_libraries(${LIB_NAME}
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
            target_link_libraries(${LIB_NAME}
                gstreamer-full-1.0
                gio-2.0
                glib-2.0
                gmodule-2.0
                gobject-2.0)
        endif ()

        if (PORT MATCHES "UltralightWin")
            target_link_libraries(${LIB_NAME}
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

    if (MSVC)
        if (NOT ${ALLINONE_BUILD})
            add_custom_command(TARGET ${LIB_NAME} POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHTCORE_DIR}/bin/" $<TARGET_FILE_DIR:${LIB_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${WEBCORE_DIR}/bin/" $<TARGET_FILE_DIR:${LIB_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_DIR}/bin/" $<TARGET_FILE_DIR:${LIB_NAME}>)
        endif()
    endif()
    
    set(RESOURCES_PATH "$<TARGET_FILE_DIR:${LIB_NAME}>") 

    # Copy assets to assets path
    add_custom_command(TARGET ${LIB_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/tests/${LIB_NAME}/assets/" "${RESOURCES_PATH}/assets/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${WEBCORE_DIR}/bin/resources/" "${RESOURCES_PATH}/assets/resources/")

    if (APPLE)
        if (NOT ${ALLINONE_BUILD})
            add_custom_command(TARGET ${LIB_NAME} POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHTCORE_DIR}/bin/libUltralightCore.dylib" $<TARGET_FILE_DIR:${LIB_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy "${WEBCORE_DIR}/bin/*.dylib" $<TARGET_FILE_DIR:${LIB_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHT_DIR}/bin/libUltralight.dylib" $<TARGET_FILE_DIR:${LIB_NAME}>)
        endif()
    endif ()

    if (PORT MATCHES "UltralightLinux")
        set_target_properties(${LIB_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath-link,${WEBCORE_DIR}/bin/")

        if (NOT ${ALLINONE_BUILD})
            add_custom_command(TARGET ${LIB_NAME} POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHTCORE_DIR}/bin/libUltralightCore.so" $<TARGET_FILE_DIR:${LIB_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy "${WEBCORE_DIR}/bin/lib*" $<TARGET_FILE_DIR:${LIB_NAME}>
                COMMAND ${CMAKE_COMMAND} -E copy "${ULTRALIGHT_DIR}/bin/libUltralight.so" $<TARGET_FILE_DIR:${LIB_NAME}>)
        endif()
    endif ()

    if (${ALLINONE_BUILD})
        add_custom_command(TARGET ${LIB_NAME} POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${LIB_NAME}>" ${CMAKE_INSTALL_PREFIX}/bin)
        add_custom_command(TARGET ${LIB_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/tests/${LIB_NAME}/assets/" "${CMAKE_INSTALL_PREFIX}/bin/assets"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${WEBCORE_DIR}/bin/resources/" "${CMAKE_INSTALL_PREFIX}/bin/assets/resources/")
    endif()

endmacro()