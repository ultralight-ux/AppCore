#include <AppCore/Platform.h>

namespace ultralight {

FontLoader* CreatePlatformFontLoader();
FileSystem* CreatePlatformFileSystem(const String& baseDir);

static FontLoader* g_font_loader = 0;
static FileSystem* g_file_system = 0;

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

}  // namespace ultralight
