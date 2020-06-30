#include <AppCore/Platform.h>
#include "FileLogger.h"

namespace ultralight {

FontLoader* CreatePlatformFontLoader();
FileSystem* CreatePlatformFileSystem(const String& baseDir);

static FontLoader* g_font_loader = 0;
static FileSystem* g_file_system = 0;
static Logger* g_logger = 0;

FontLoader* GetPlatformFontLoader() {
  if (!g_font_loader) {
    g_font_loader = CreatePlatformFontLoader();
  }

  return g_font_loader;
}

FileSystem* GetPlatformFileSystem(const String& baseDir) {
  if (!g_file_system) {
    g_file_system = CreatePlatformFileSystem(baseDir);
  }

  return g_file_system;
}

Logger* GetDefaultLogger(const String& logPath) {
  if (!g_logger) {
    g_logger = new FileLogger(logPath);
  }

  return g_logger;
}

}  // namespace ultralight
