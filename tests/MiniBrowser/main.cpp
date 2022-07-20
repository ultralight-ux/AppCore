#include "Browser.h"

#define ENABLE_PAUSE_FOR_DEBUGGER 0

#if defined(_WIN32) && ENABLE_PAUSE_FOR_DEBUGGER
#include <Windows.h>
void PauseForDebugger() { MessageBoxA(NULL, "Pause", "Caption", MB_OKCANCEL); }
#else
void PauseForDebugger() {}
#endif

int main() {
  PauseForDebugger();
  
  Browser browser;
  browser.Run();
  return 0;
}
