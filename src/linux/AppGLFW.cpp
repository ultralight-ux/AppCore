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

extern "C" {

static void GLFW_error_callback(int error, const char* description) {
  fprintf(stderr, "GLFW Error: %s\n", description);
}

static const char* GetHomeDirectory() {
  const char* result;
  if (!(result = getenv("HOME")))
    result = getpwuid(getuid())->pw_dir;
  return result;
}

static const char* GetExecutableDirectory() {
  static char exe_path[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", exe_path, PATH_MAX);
  const char* result = "";
  if (count != -1)
    result = dirname(exe_path);
  return result;
}

}

namespace ultralight {

AppGLFW::AppGLFW(Settings settings, Config config) : settings_(settings) {
  // Generate cache path
  String cache_path = GetHomeDirectory();
  cache_path = PlatformFileSystem::AppendPath(cache_path, ".cache");
  String cache_dirname = settings_.developer_name + "-" +
    settings_.app_name;
  cache_path = PlatformFileSystem::AppendPath(cache_path, cache_dirname);
  PlatformFileSystem::MakeAllDirectories(cache_path);

  if (!Platform::instance().logger()) {
    String log_path = PlatformFileSystem::AppendPath(cache_path,
                                                    "ultralight.log");

    std::cout << "Writing log to: " << log_path.utf8().data() << std::endl;

    logger_.reset(new FileLogger(log_path));
    Platform::instance().set_logger(logger_.get());
  }

  // Determine resources path
  String executable_path = GetExecutableDirectory();
  String resource_path = PlatformFileSystem::AppendPath(executable_path, "resources/");

  config.cache_path = cache_path.utf16();
  config.resource_path = resource_path.utf16();

  glfwSetErrorCallback(GLFW_error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  main_monitor_.reset(new MonitorGLFW());

  config.face_winding = kFaceWinding_Clockwise;
  Platform::instance().set_config(config);

  if (!Platform::instance().file_system()) {
    // Determine file system path
    String file_system_path = PlatformFileSystem::AppendPath(executable_path, 
      settings_.file_system_path.utf16());

    Platform::instance().set_file_system(GetPlatformFileSystem(file_system_path));
  

    std::ostringstream info;
    info << "File system base directory resolved to: " <<
      file_system_path.utf8().data();
    UL_LOG_INFO(info.str().c_str());
  }

  if (!Platform::instance().font_loader()) {
    Platform::instance().set_font_loader(GetPlatformFontLoader());
  }

  clipboard_.reset(new ClipboardGLFW());
  Platform::instance().set_clipboard(clipboard_.get());

  gpu_context_.reset(new GPUContextGL(true, false));
  Platform::instance().set_gpu_driver(gpu_context_->driver());

  // We use the GPUContext's global offscreen window to maintain
  // clipboard state for the duration of the app lifetime.
  clipboard_->set_window(gpu_context_->window());

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

Ref<Renderer> AppGLFW::renderer() {
  return *renderer_.get();
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
}

static App* g_app_instance = nullptr;

Ref<App> App::Create(Settings settings, Config config) {
  g_app_instance = new AppGLFW(settings, config);
  return AdoptRef(*g_app_instance);
}

App::~App() {
  g_app_instance = nullptr;
}

App* App::instance() {
  return g_app_instance;
}

}  // namespace ultralight
