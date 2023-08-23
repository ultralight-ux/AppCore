#include <AppCore/CAPI.h>
#include <AppCore/AppCore.h>
#include <AppCore/Platform.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>

using namespace ultralight;

///
/// Undocumented functions from Ultralight's CAPI
///
ULExport ULRenderer C_WrapRenderer(RefPtr<Renderer> renderer); // Must be destroyed
ULExport ULView C_WrapView(RefPtr<View> view); // Must be destroyed
ULExport RefPtr<Renderer> C_UnwrapRenderer(ULRenderer renderer);
ULExport RefPtr<View> C_UnwrapView(ULView view);

#define ToULString(x) reinterpret_cast<ULString>(const_cast<ultralight::String*>(&x))
#define ToString(x) (*reinterpret_cast<ultralight::String*>(x))

struct C_Settings {
  Settings val;
};

struct C_Config {
  Config val;
};

struct C_App : public AppListener {
	RefPtr<App> val;
  ULRenderer c_renderer = nullptr; // owned by C_App, destroyed in destructor
  ULWindow c_window = nullptr; // owned by user, cached in ULAppSetWindow
  ULUpdateCallback update_callback = nullptr;
  void* update_callback_data = nullptr;

  C_App(RefPtr<App> app) : val(app) {
    val->set_listener(this);
    c_renderer = C_WrapRenderer(app->renderer());
  }

  virtual ~C_App() {
    ulDestroyRenderer(c_renderer);
    val->set_listener(nullptr);
  }

  virtual void OnUpdate() override {
    if (update_callback)
      update_callback(update_callback_data);
  }
};

struct C_Window : public WindowListener {
  RefPtr<Window> val;
  ULCloseCallback close_callback = nullptr;
  void* close_callback_data = nullptr;
  ULResizeCallback resize_callback = nullptr;
  void* resize_callback_data = nullptr;

  C_Window(RefPtr<Window> window) : val(window) {
    window->set_listener(this);
  }

  virtual ~C_Window() {
    val->set_listener(nullptr);
  }

  virtual void OnClose(Window* window) override {
    if (close_callback)
      close_callback(close_callback_data, this);
  }

  virtual void OnResize(Window* window, uint32_t width, uint32_t height) override {
    if (resize_callback)
      resize_callback(resize_callback_data, this, width, height);
  }
};

struct C_Overlay {
  RefPtr<Overlay> val;
  ULView c_view = nullptr;

  C_Overlay(RefPtr<Overlay> overlay) : val(overlay) {
    c_view = C_WrapView(val->view());
  }

  virtual ~C_Overlay() {
    ulDestroyView(c_view);
  }
};

ULSettings ulCreateSettings() {
  return new C_Settings();
}

void ulDestroySettings(ULSettings settings) {
  delete settings;
}

void ulSettingsSetDeveloperName(ULSettings settings, ULString name) {
  settings->val.developer_name = ToString(name).utf16();
}

void ulSettingsSetAppName(ULSettings settings, ULString name) {
  settings->val.app_name = ToString(name).utf16();
}

void ulSettingsSetFileSystemPath(ULSettings settings, ULString path) {
  settings->val.file_system_path = ToString(path).utf16();
}

void ulSettingsSetLoadShadersFromFileSystem(ULSettings settings, bool enabled) {
  settings->val.load_shaders_from_file_system = enabled;
}

void ulSettingsSetForceCPURenderer(ULSettings settings, bool force_cpu) {
  settings->val.force_cpu_renderer = force_cpu;
}

ULApp ulCreateApp(ULSettings settings, ULConfig config) {
  return new C_App{ App::Create(settings ? settings->val : Settings(),
                                config ? config->val : Config()) };
}

void ulDestroyApp(ULApp app) {
  delete app;
}

void ulAppSetUpdateCallback(ULApp app, ULUpdateCallback callback, void* user_data) {
  app->update_callback = callback;
  app->update_callback_data = user_data;
}

bool ulAppIsRunning(ULApp app) {
  return app->val->is_running();
}

ULMonitor ulAppGetMainMonitor(ULApp app) {
  return reinterpret_cast<ULMonitor>(app->val->main_monitor());
}

ULRenderer ulAppGetRenderer(ULApp app) {
  return app->c_renderer;
}

void ulAppRun(ULApp app) {
  app->val->Run();
}

void ulAppQuit(ULApp app) {
  app->val->Quit();
}

double ulMonitorGetScale(ULMonitor monitor) {
  return reinterpret_cast<Monitor*>(monitor)->scale();
}

unsigned int ulMonitorGetWidth(ULMonitor monitor) {
  return reinterpret_cast<Monitor*>(monitor)->width();
}

unsigned int ulMonitorGetHeight(ULMonitor monitor) {
  return reinterpret_cast<Monitor*>(monitor)->height();
}

ULWindow ulCreateWindow(ULMonitor monitor, unsigned int width,
  unsigned int height, bool fullscreen, unsigned int window_flags) {
  return new C_Window(Window::Create(reinterpret_cast<Monitor*>(monitor),
    width, height, fullscreen, window_flags));
}

void ulDestroyWindow(ULWindow window) {
  delete window;
}

void ulWindowSetCloseCallback(ULWindow window, ULCloseCallback callback, void* user_data) {
  window->close_callback = callback;
  window->close_callback_data = user_data;
}

void ulWindowSetResizeCallback(ULWindow window, ULResizeCallback callback, void* user_data) {
  window->resize_callback = callback;
  window->resize_callback_data = user_data;
}

unsigned int ulWindowGetScreenWidth(ULWindow window) {
  return window->val->screen_width();
}

unsigned int ulWindowGetWidth(ULWindow window) { return window->val->width(); }

unsigned int ulWindowGetScreenHeight(ULWindow window) {
  return window->val->screen_height();
}

unsigned int ulWindowGetHeight(ULWindow window) { return window->val->height(); }

void ulWindowMoveTo(ULWindow window, int x, int y) { window->val->MoveTo(x, y); }

void ulWindowMoveToCenter(ULWindow window) { window->val->MoveToCenter(); }

int ulWindowGetPositionX(ULWindow window) { return window->val->x(); }

int ulWindowGetPositionY(ULWindow window) { return window->val->y(); }

bool ulWindowIsFullscreen(ULWindow window) {
  return window->val->is_fullscreen();
}

bool ulWindowIsAccelerated(ULWindow window) {
  return window->val->is_accelerated();
}

unsigned int ulWindowGetRenderBufferId(ULWindow window) {
  return window->val->render_buffer_id();
}

double ulWindowGetScale(ULWindow window) {
  return window->val->scale();
}

void ulWindowSetTitle(ULWindow window, const char* title) {
  window->val->SetTitle(title);
}

void ulWindowSetCursor(ULWindow window, ULCursor cursor) {
  window->val->SetCursor((Cursor)cursor);
}

void ulWindowShow(ULWindow window) { window->val->Show(); }

void ulWindowHide(ULWindow window) { window->val->Hide(); }

bool ulWindowIsVisible(ULWindow window) { return window->val->is_visible(); }

void ulWindowClose(ULWindow window) {
  window->val->Close();
}

int ulWindowScreenToPixels(ULWindow window, int val) {
  return window->val->ScreenToPixels(val);
}

int ulWindowPixelsToScreen(ULWindow window, int val) {
  return window->val->PixelsToScreen(val);
}

void* ulWindowGetNativeHandle(ULWindow window) {
  return window->val->native_handle();
}

ULOverlay ulCreateOverlay(ULWindow window, unsigned int width,
                          unsigned int height, int x, int y) {
  return new C_Overlay(Overlay::Create(window->val, width, height, x, y));
}

ULOverlay ulCreateOverlayWithView(ULWindow window, ULView view,
  int x, int y) {
  return new C_Overlay(Overlay::Create(window->val, C_UnwrapView(view), x, y));
}

void ulDestroyOverlay(ULOverlay overlay) {
  delete overlay;
}

ULView ulOverlayGetView(ULOverlay overlay) {
  return overlay->c_view;
}

unsigned int ulOverlayGetWidth(ULOverlay overlay) {
  return overlay->val->width();
}

unsigned int ulOverlayGetHeight(ULOverlay overlay) {
  return overlay->val->height();
}

int ulOverlayGetX(ULOverlay overlay) {
  return overlay->val->x();
}

int ulOverlayGetY(ULOverlay overlay) {
  return overlay->val->y();
}

void ulOverlayMoveTo(ULOverlay overlay, int x, int y) {
  overlay->val->MoveTo(x, y);
}

void ulOverlayResize(ULOverlay overlay, unsigned int width,
                     unsigned int height) {
  overlay->val->Resize(width, height);
}

bool ulOverlayIsHidden(ULOverlay overlay) {
  return overlay->val->is_hidden();
}

void ulOverlayHide(ULOverlay overlay) {
  overlay->val->Hide();
}

void ulOverlayShow(ULOverlay overlay) {
  overlay->val->Show();
}

bool ulOverlayHasFocus(ULOverlay overlay) {
  return overlay->val->has_focus();
}

void ulOverlayFocus(ULOverlay overlay) {
  overlay->val->Focus();
}

void ulOverlayUnfocus(ULOverlay overlay) {
  overlay->val->Unfocus();
}

void ulEnablePlatformFontLoader() {
  Platform::instance().set_font_loader(GetPlatformFontLoader());
}

void ulEnablePlatformFileSystem(ULString base_dir) {
  ultralight::String str;
  if (base_dir)
    str = ToString(base_dir);

  Platform::instance().set_file_system(GetPlatformFileSystem(str));
}

void ulEnableDefaultLogger(ULString log_path) {
  ultralight::String str = ToString(log_path);

  Platform::instance().set_logger(GetDefaultLogger(str));
}
