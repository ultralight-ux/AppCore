#include "AppGLFW.h"
#include "gl/GPUContextGL.h"
#include "gl/GPUDriverGL.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>
#include <GLFW/glfw3.h>
#include "WindowGLFW.h"
#include <AppCore/Platform.h>
#include "ClipboardGLFW.h"
#include "FileLogger.h"
#include <Ultralight/private/util/Debug.h>
#include <Ultralight/private/PlatformFileSystem.h>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <libgen.h>
#include <iostream>
#include <filesystem>

extern "C" {

static void GLFW_error_callback(int error, const char* description) {
  fprintf(stderr, "GLFW Error: %s\n", description);
}

std::filesystem::path GetCacheDirectory() {
    const char* xdgCacheHome = std::getenv("XDG_CACHE_HOME");
    if (xdgCacheHome) {
        return std::filesystem::path(xdgCacheHome);
    } else {
        const char* home = std::getenv("HOME");
        if (home) {
            return std::filesystem::path(home) / ".cache";
        }
    }
    // Handle error or use a default path
    return std::filesystem::path("/tmp");
}

static std::filesystem::path GetExecutableDirectory() {
  static char exe_path[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", exe_path, PATH_MAX);
  const char* result = "";
  if (count != -1)
    result = dirname(exe_path);
  return std::filesystem::path(result);
}

}

namespace ultralight {

AppGLFW::AppGLFW(Settings settings, Config config) : settings_(settings) {
  // Generate cache path
  std::filesystem::path cache_path = GetCacheDirectory();
  String cache_dirname = "com." + settings_.developer_name + "." +
    settings_.app_name;
  cache_path /= std::string(cache_dirname.utf8().data());
  if (!std::filesystem::exists(cache_path)) {
    if (!std::filesystem::create_directories(cache_path)) {
      std::cout << "Failed to create cache path: " << cache_path.string() << std::endl;
    }
  }

  if (!Platform::instance().logger()) {
    std::filesystem::path log_path = cache_path / std::filesystem::path("ultralight.log");
    logger_.reset(new FileLogger(log_path.string().c_str()));
    Platform::instance().set_logger(logger_.get());
  }

  // Determine resources path
  std::filesystem::path executable_path = GetExecutableDirectory();

  config.cache_path = cache_path.string().c_str();

  glfwSetErrorCallback(GLFW_error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  main_monitor_.reset(new MonitorGLFW());

  config.face_winding = FaceWinding::Clockwise;
  Platform::instance().set_config(config);

  if (!Platform::instance().file_system()) {
    // Determine file system path
    std::string fs_str = settings.file_system_path.utf8().data();
    std::filesystem::path file_system_path = executable_path / std::filesystem::path(fs_str);

    Platform::instance().set_file_system(GetPlatformFileSystem(file_system_path.string().c_str()));
  }

  if (!Platform::instance().font_loader()) {
    Platform::instance().set_font_loader(GetPlatformFontLoader());
  }

  clipboard_.reset(new ClipboardGLFW());
  Platform::instance().set_clipboard(clipboard_.get());

  gpu_context_.reset(new GPUContextGL(true, true));
  Platform::instance().set_gpu_driver(gpu_context_->driver());

  // We use the GPUContext's global offscreen window to maintain
  // clipboard state for the duration of the app lifetime.
  clipboard_->set_window(gpu_context_->window());

  surface_factory_.reset(new ULTextureSurfaceFactory());
  Platform::instance().set_surface_factory(surface_factory_.get());

  renderer_ = Renderer::Create();
}

AppGLFW::~AppGLFW() {
  windows_.clear();
  gpu_context_.reset();
  Platform::instance().set_gpu_driver(nullptr);
  glfwTerminate();
}

Monitor* AppGLFW::main_monitor() {
  return main_monitor_.get();
}

RefPtr<Renderer> AppGLFW::renderer() {
  return renderer_;
}

void AppGLFW::Run() {
  if (is_running_)
   return;

  is_running_ = true;

  ///
  /// Our main run loop tries to conserve CPU usage by sleeping in 4ms bursts
  /// between each paint.
  ///
  /// We use glfwWaitEventsTimeout() to perform the sleep, which also allows
  /// us the ability to wake up early if the OS sends us an event.
  ///
  std::chrono::milliseconds interval_ms(4);
  std::chrono::steady_clock::time_point next_paint = std::chrono::steady_clock::now();
  
  while (is_running_) {
    ///
    /// Query the system clock to see how many milliseconds are left until
    /// the next scheduled paint.
    ///
    long long timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
      next_paint - std::chrono::steady_clock::now()).count();
    unsigned long timeout = timeout_ms <= 0 ? 0 : (unsigned long)timeout_ms;

    ///
    /// Use glfwWaitEventsTimeout() if we still have time left before the next
    /// paint. (Will use OS primitives to sleep and wait for OS input events)
    ///
    /// Otherwise, call glfwPollEvents() immediately and continue.
    ///
    if (timeout > 0)
      glfwWaitEventsTimeout(timeout / 1000.0);
    else
      glfwPollEvents();

    ///
    /// Allow Ultralight to update internal timers, JavaScript callbacks, and
    /// other resource callbacks.
    ///
    Update();

    if (!is_running_)
      return;

    ///
    /// Update our timeout by querying the system clock again.
    ///
    timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
      next_paint - std::chrono::steady_clock::now()).count();

    ///
    /// Perform paint if we have reached the scheduled time.
    ///
    if (timeout_ms <= 0) {
      for (auto window : windows_) {
        if (window->NeedsRepaint())
          window->Repaint();
      }

      ///
      /// Schedule the next paint.
      ///
      next_paint = std::chrono::steady_clock::now() + interval_ms;
    }
  }
}

void AppGLFW::Quit() {
  is_running_ = false;
}

void AppGLFW::Update() {
  if (listener_)
    listener_->OnUpdate();

  renderer()->Update();

  App::instance()->renderer()->RefreshDisplay(0);
}

static App* g_app_instance = nullptr;

RefPtr<App> App::Create(Settings settings, Config config) {
  g_app_instance = (App*)new AppGLFW(settings, config);
  return AdoptRef(*g_app_instance);
}

App::~App() {
  g_app_instance = nullptr;
}

App* App::instance() {
  return g_app_instance;
}

}  // namespace ultralight
