#include "AppGLFW.h"
#include "gl/GPUContextGL.h"
#include "gl/GPUDriverGL.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>
#include <GLFW/glfw3.h>
#include "WindowGLFW.h"
#include "FileSystemBasic.h"
#include "FontLoaderLinux.h"
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

  // Force GPU renderer by default until we support CPU drawing in this port
  config.use_gpu_renderer = true;

  // Generate cache path
  String cache_path = GetHomeDirectory();
  cache_path = PlatformFileSystem::AppendPath(cache_path, ".cache");
  String cache_dirname = settings_.developer_name + "-" +
    settings_.app_name;
  cache_path = PlatformFileSystem::AppendPath(cache_path, cache_dirname);
  PlatformFileSystem::MakeAllDirectories(cache_path);

  String log_path = PlatformFileSystem::AppendPath(cache_path,
                                                   "ultralight.log");

  std::cout << log_path.utf8().data() << std::endl;

  logger_.reset(new FileLogger(log_path));
  Platform::instance().set_logger(logger_.get());

  // Determine resources path
  String executable_path = GetExecutableDirectory();
  String resource_path = PlatformFileSystem::AppendPath(executable_path, "resources/");

  config.cache_path = cache_path.utf16();
  config.resource_path = resource_path.utf16();

  glfwSetErrorCallback(GLFW_error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  main_monitor_.reset(new MonitorGLFW());

  config.device_scale = main_monitor_->scale();
  config.face_winding = kFaceWinding_Clockwise;
  Platform::instance().set_config(config);

  // Determine file system path
  String file_system_path = PlatformFileSystem::AppendPath(executable_path, 
    settings_.file_system_path.utf16());

  file_system_.reset(new FileSystemBasic(file_system_path.utf8().data()));
  Platform::instance().set_file_system(file_system_.get());

  std::ostringstream info;
  info << "File system base directory resolved to: " <<
    file_system_path.utf8().data();
  UL_LOG_INFO(info.str().c_str());

  font_loader_.reset(new FontLoaderLinux());
  Platform::instance().set_font_loader(font_loader_.get());

  clipboard_.reset(new ClipboardGLFW());
  Platform::instance().set_clipboard(clipboard_.get());

  renderer_ = Renderer::Create();
}

AppGLFW::~AppGLFW() {
  window_ = nullptr;
  glfwTerminate();
}

void AppGLFW::OnClose() {
}

void AppGLFW::OnResize(uint32_t width, uint32_t height) {
  if (gpu_context_)
    gpu_context_->Resize((int)width, (int)height);
}

void AppGLFW::set_window(Ref<Window> window) {
  window_ = window;
  
  WindowGLFW* win = static_cast<WindowGLFW*>(window_.get());
  gpu_context_.reset(new GPUContextGL(win->handle(), (float)win->scale(), true, true));
  Platform::instance().set_gpu_driver(gpu_context_->driver());
  
  win->set_app_listener(this);

  clipboard_->set_window(win->handle());
}

Monitor* AppGLFW::main_monitor() {
  return main_monitor_.get();
}

Ref<Renderer> AppGLFW::renderer() {
  return *renderer_.get();
}

void AppGLFW::Run() {
  if (!window_) {
    std::printf("Forgot to call App::set_window before App::Run\n");
    exit(-1);
  }

  WindowGLFW* win = static_cast<WindowGLFW*>(window_.get());

  while (!glfwWindowShouldClose(win->handle())) {
    glfwPollEvents();
    Update();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void AppGLFW::Quit() {
  is_running_ = false;
}

void AppGLFW::Update() {
  if (listener_)
    listener_->OnUpdate();

  renderer()->Update();

  GPUDriverImpl* driver = gpu_context_->driver();

  driver->BeginSynchronize();
  renderer_->Render();
  driver->EndSynchronize();

  if (driver->HasCommandsPending()) {
    gpu_context_->BeginDrawing();
    driver->DrawCommandList();
    if (window_)
	    static_cast<WindowGLFW*>(window_.get())->Draw();
    gpu_context_->PresentFrame();
    gpu_context_->EndDrawing();
  }
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
