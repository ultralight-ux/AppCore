//========================================================================
// Portions of the below file are under the following license:
//
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2016 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#include "WindowsUtil.h"

namespace ultralight {

// HACK: Define versionhelpers.h functions manually as MinGW lacks the header
static BOOL IsWindowsVersionOrGreater(WORD major, WORD minor, WORD sp);
#define IsWindowsXPOrGreater()                                 \
    IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP),      \
                              LOBYTE(_WIN32_WINNT_WINXP), 0)
#define IsWindowsVistaOrGreater()                              \
    IsWindowsVersionOrGreaterWin32(HIBYTE(_WIN32_WINNT_VISTA),      \
                              LOBYTE(_WIN32_WINNT_VISTA), 0)
#define IsWindows7OrGreater()                                  \
    IsWindowsVersionOrGreaterWin32(HIBYTE(_WIN32_WINNT_WIN7),       \
                              LOBYTE(_WIN32_WINNT_WIN7), 0)
#define IsWindows8OrGreater()                                  \
    IsWindowsVersionOrGreaterWin32(HIBYTE(_WIN32_WINNT_WIN8),       \
                              LOBYTE(_WIN32_WINNT_WIN8), 0)
#define IsWindows8Point1OrGreater()                            \
    IsWindowsVersionOrGreaterWin32(HIBYTE(_WIN32_WINNT_WINBLUE),    \
                              LOBYTE(_WIN32_WINNT_WINBLUE), 0)

WindowsUtil::WindowsUtil() {
  shcore_lib_ = LoadLibraryA("shcore.dll");
  if (shcore_lib_) {
    setProcessDpiAwareness_ = (FN_SetProcessDpiAwareness)
      GetProcAddress(shcore_lib_, "SetProcessDpiAwareness");
    getDpiForMonitor_ = (FN_GetDpiForMonitor)
      GetProcAddress(shcore_lib_, "GetDpiForMonitor");
  }

  ntdll_lib_ = LoadLibraryA("ntdll.dll");
  if (ntdll_lib_) {
    rtlVerifyVersionInfo_
        = (FN_RtlVerifyVersionInfo)GetProcAddress(ntdll_lib_, "RtlVerifyVersionInfo");
  }
}

WindowsUtil::~WindowsUtil() {
  if (shcore_lib_)
    FreeLibrary(shcore_lib_);
  if (ntdll_lib_)
    FreeLibrary(ntdll_lib_);
}

void WindowsUtil::EnableDPIAwareness() {
  if (IsWindows8Point1OrGreater())
    setProcessDpiAwareness_(PROCESS_PER_MONITOR_DPI_AWARE);
  else if (IsWindowsVistaOrGreater())
    SetProcessDPIAware();
}

double WindowsUtil::GetMonitorDPI(HMONITOR monitor) {
  UINT xdpi, ydpi;

  if (IsWindows8Point1OrGreater()) {
    getDpiForMonitor_(monitor, MDT_EFFECTIVE_DPI, &xdpi, &ydpi);
  } else {
    const HDC dc = GetDC(NULL);
    xdpi = GetDeviceCaps(dc, LOGPIXELSX);
    ydpi = GetDeviceCaps(dc, LOGPIXELSY);
    ReleaseDC(NULL, dc);
  }

  // We only care about DPI in x-dimension right now
  return xdpi / 96.f;
}

BOOL WindowsUtil::IsWindowsVersionOrGreaterWin32(WORD major, WORD minor, WORD sp) {
  if (!rtlVerifyVersionInfo_)
    return false;

  OSVERSIONINFOEXW osvi = { sizeof(osvi), major, minor, 0, 0, { 0 }, sp };
  DWORD mask = VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR;
  ULONGLONG cond = VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
  cond = VerSetConditionMask(cond, VER_MINORVERSION, VER_GREATER_EQUAL);
  cond = VerSetConditionMask(cond, VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
  // HACK: Use RtlVerifyVersionInfo instead of VerifyVersionInfoW as the
  //       latter lies unless the user knew to embed a non-default manifest
  //       announcing support for Windows 10 via supportedOS GUID
  return rtlVerifyVersionInfo_(&osvi, mask, cond) == 0;
}

// Checks whether we are on at least the specified build of Windows 10
//
BOOL WindowsUtil::IsWindows10BuildOrGreaterWin32(WORD build) {
  if (!rtlVerifyVersionInfo_)
    return false;

  OSVERSIONINFOEXW osvi = { sizeof(osvi), 10, 0, build };
  DWORD mask = VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER;
  ULONGLONG cond = VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
  cond = VerSetConditionMask(cond, VER_MINORVERSION, VER_GREATER_EQUAL);
  cond = VerSetConditionMask(cond, VER_BUILDNUMBER, VER_GREATER_EQUAL);
  // HACK: Use RtlVerifyVersionInfo instead of VerifyVersionInfoW as the
  //       latter lies unless the user knew to embed a non-default manifest
  //       announcing support for Windows 10 via supportedOS GUID
  return rtlVerifyVersionInfo_(&osvi, mask, cond) == 0;
}

}  // namespace ultralight
