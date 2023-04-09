#include "MiniBrowserLib.h"
#include <cassert>

#include "Browser.h"
#include <Ultralight/platform/Allocator.h>

static Browser* g_browser = nullptr;

#if defined(_WIN32)
#include "ThreadFactoryWin.h"
#include <Ultralight/platform/Platform.h>
static ThreadFactoryWin* thread_factory = nullptr;
#endif

void* MiniBrowser_Initialize() {
  assert(g_browser == nullptr);

#if defined(_WIN32)
  thread_factory = new ThreadFactoryWin();
  ultralight::Platform::instance().set_thread_factory(thread_factory);
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
  ultralight::Platform::instance().set_thread_factory(nullptr);
  delete thread_factory;
  thread_factory = nullptr;
#endif
}
