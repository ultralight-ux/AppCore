#ifndef BUILDING_AS_LIBRARY

#include "Browser.h"

#define ENABLE_PAUSE_FOR_DEBUGGER 1

#if defined(_WIN32) && ENABLE_PAUSE_FOR_DEBUGGER
#include <Windows.h>
void PauseForDebugger() { MessageBoxA(NULL, "Pause", "Caption", MB_OKCANCEL); }
#else
void PauseForDebugger() {}
#endif

#if defined(_WIN32)
#include "ThreadFactoryWin.h"
#include <Ultralight/platform/Platform.h>
#endif

int main() {
  PauseForDebugger();

#if defined(_WIN32)
  ThreadFactoryWin* thread_factory = new ThreadFactoryWin();
  ultralight::Platform::instance().set_thread_factory(thread_factory);
#endif
  
  {
    Browser browser;
    browser.Run();
  }

#if defined(_WIN32)
  ultralight::Platform::instance().set_thread_factory(nullptr);
  delete thread_factory;
#endif

  PauseForDebugger();

  return 0;
}

#endif