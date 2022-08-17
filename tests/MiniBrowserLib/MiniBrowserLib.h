#ifndef MINI_BROWSER_LIB_CAPI_H
#define MINI_BROWSER_LIB_CAPI_H

#  if defined(__WIN32__) || defined(_WIN32)
#    if defined(APPCORE_IMPLEMENTATION)
#      define ACExport __declspec(dllexport)
#    else
#      define ACExport __declspec(dllimport)
#    endif
#  else
#    define ACExport __attribute__((visibility("default")))
#  endif

#ifdef __cplusplus
extern "C" {
#endif

// Returns handle to the private heap (Windows)
ACExport void* MiniBrowser_Initialize();

ACExport void MiniBrowser_Run();

ACExport void MiniBrowser_Shutdown();

#ifdef __cplusplus
}
#endif

#endif // MINI_BROWSER_LIB_CAPI_H