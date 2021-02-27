#pragma once
#include <Windows.h>

// Enum from <ShellScalingAPI.h> (Shcore.lib)
typedef enum
{
  PROCESS_DPI_UNAWARE = 0,
  PROCESS_SYSTEM_DPI_AWARE = 1,
  PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

// Enum from <ShellScalingAPI.h> (Shcore.lib)
typedef enum
{
  MDT_EFFECTIVE_DPI = 0,
  MDT_ANGULAR_DPI = 1,
  MDT_RAW_DPI = 2,
  MDT_DEFAULT = MDT_EFFECTIVE_DPI
} MONITOR_DPI_TYPE;

namespace ultralight {
class WindowsUtil {
public:
  WindowsUtil();
  ~WindowsUtil();

  void EnableDPIAwareness();

  double GetMonitorDPI(HMONITOR monitor);

protected:

  BOOL IsWindowsVersionOrGreaterWin32(WORD major, WORD minor, WORD sp);

  BOOL IsWindows10BuildOrGreaterWin32(WORD build);

  typedef HRESULT(WINAPI * FN_SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS);
  typedef HRESULT(WINAPI * FN_GetDpiForMonitor)(HMONITOR, MONITOR_DPI_TYPE, UINT*, UINT*);
  typedef LONG(WINAPI* FN_RtlVerifyVersionInfo)(OSVERSIONINFOEXW*, ULONG, ULONGLONG);
  FN_SetProcessDpiAwareness setProcessDpiAwareness_ = nullptr;
  FN_GetDpiForMonitor getDpiForMonitor_ = nullptr;
  FN_RtlVerifyVersionInfo rtlVerifyVersionInfo_ = nullptr;
  HMODULE shcore_lib_ = nullptr;
  HMODULE ntdll_lib_ = nullptr;
};

}