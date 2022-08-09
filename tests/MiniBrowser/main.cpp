#include "Browser.h"

#define ENABLE_PAUSE_FOR_DEBUGGER 0

#if defined(_WIN32) && ENABLE_PAUSE_FOR_DEBUGGER
#include <Windows.h>
void PauseForDebugger() { MessageBoxA(NULL, "Pause", "Caption", MB_OKCANCEL); }
#else
void PauseForDebugger() {}
#endif

#if defined(_WIN32)
#include "ThreadManagerWin.h"
#include <Ultralight/platform/Platform.h>
#endif

int main() {
  PauseForDebugger();

#if defined(_WIN32)
  ThreadManagerWin* thread_manager = new ThreadManagerWin();
  ultralight::Platform::instance().set_thread_manager(thread_manager);
#endif
  
  {
    Browser browser;
    browser.Run();
  }

#if defined(_WIN32)
  ultralight::Platform::instance().set_thread_manager(nullptr);
  delete thread_manager;
#endif

  return 0;
}
