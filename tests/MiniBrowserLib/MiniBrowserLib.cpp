#include "MiniBrowserLib.h"
#include <cassert>

#include "Browser.h"
#include <Ultralight/platform/Allocator.h>

static Browser* g_browser = nullptr;

#if defined(_WIN32)
#include "ThreadManagerWin.h"
#include <Ultralight/platform/Platform.h>
static ThreadManagerWin* thread_manager = nullptr;
#endif

void* MiniBrowser_Initialize() {
  assert(g_browser == nullptr);

#if defined(_WIN32)
  thread_manager = new ThreadManagerWin();
  ultralight::Platform::instance().set_thread_manager(thread_manager);
#endif

  g_browser = new Browser();

#if defined(_WIN32)
  return ulGetHeapHandle();
#else
  return nullptr;
#endif
}

void MiniBrowser_Run() {
  assert(g_browser);
  g_browser->Run();
}

void MiniBrowser_Shutdown() {
  assert(g_browser);
  delete g_browser;
  g_browser = nullptr;

#if defined(_WIN32)
  ultralight::Platform::instance().set_thread_manager(nullptr);
  delete thread_manager;
  thread_manager = nullptr;
#endif
}
