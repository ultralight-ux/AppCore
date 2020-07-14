if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
else ()
    set(ARCHITECTURE "x86")
endif ()

if (PORT MATCHES "UltralightLinux")
    set(PLATFORM "linux")
elseif (PORT MATCHES "UltralightMac")
    set(PLATFORM "mac")
elseif (PORT MATCHES "UltralightWin")
    set(PLATFORM "win")
endif ()

set(ULTRALIGHTCORE_REV "ba7aa65f")
set(WEBCORE_REV "889bd9b6")
set(ULTRALIGHT_REV "781eff28")

set(ULTRALIGHTCORE_DIR "${CMAKE_SOURCE_DIR}/deps/UltralightCore/")
set(WEBCORE_DIR "${CMAKE_SOURCE_DIR}/deps/WebCore/")
set(ULTRALIGHT_DIR "${CMAKE_SOURCE_DIR}/deps/Ultralight/")

if(${USE_LOCAL_DEPS})
  add_custom_target(UltralightCoreBin)
  add_custom_target(WebCoreBin)
  add_custom_target(UltralightBin)
else ()
    ExternalProject_Add(UltralightCoreBin
      URL https://ultralightcore-bin.sfo2.digitaloceanspaces.com/ultralightcore-bin-${ULTRALIGHTCORE_REV}-${PLATFORM}-${ARCHITECTURE}.7z
      SOURCE_DIR "${ULTRALIGHTCORE_DIR}"
      BUILD_IN_SOURCE 1
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )

    ExternalProject_Add(WebCoreBin
      URL https://webcore-bin.sfo2.digitaloceanspaces.com/webcore-bin-${WEBCORE_REV}-${PLATFORM}-${ARCHITECTURE}.7z
      SOURCE_DIR "${WEBCORE_DIR}"
      BUILD_IN_SOURCE 1
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )

    ExternalProject_Add(UltralightBin
      URL https://ultralight-bin.sfo2.digitaloceanspaces.com/ultralight-bin-${ULTRALIGHT_REV}-${PLATFORM}-${ARCHITECTURE}.7z
      SOURCE_DIR "${ULTRALIGHT_DIR}"
      BUILD_IN_SOURCE 1
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )
endif ()
