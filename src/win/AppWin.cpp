#include "AppWin.h"
#include <Windows.h>
#include "WindowWin.h"
#if defined(DRIVER_D3D11)
#include "d3d11/GPUContextD3D11.h"
#include "d3d11/GPUDriverD3D11.h"
#elif defined(DRIVER_D3D12)
#include "d3d12/GPUContextD3D12.h"
#include "d3d12/GPUDriverD3D12.h"
#endif
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>
#include <Ultralight/platform/Logger.h>
#include <Ultralight/private/util/Debug.h>
#include <Ultralight/private/PlatformFileSystem.h>
#include "DIBSurface.h"
#include <Shlwapi.h>
#include <ShlObj.h>
#include "WindowsUtil.h"
#include "MonitorWin.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>

namespace ultralight {

static String GetModulePath() {
  HMODULE hModule = GetModuleHandleW(NULL);
  WCHAR module_path[MAX_PATH];
  GetModuleFileNameW(hModule, module_path, MAX_PATH);
  PathRemoveFileSpecW(module_path);
  return String16(module_path, lstrlenW(module_path));
}

static String GetRoamingAppDataPath() {
  PWSTR appDataPath = NULL;
  SHGetKnownFolderPath(FOLDERID_RoamingAppData, NULL, NULL, &appDataPath);
  String result = String16(appDataPath, lstrlenW(appDataPath));
  ::CoTaskMemFree(static_cast<void*>(appDataPath));
  return result;
}

AppWin::AppWin(Settings settings, Config config) : settings_(settings) {
  windows_util_.reset(new WindowsUtil());
  windows_util_->EnableDPIAwareness();

  main_monitor_.reset(new MonitorWin(windows_util_.get()));

  // Generate cache path
  String cache_path = GetRoamingAppDataPath();
  cache_path = PlatformFileSystem::AppendPath(cache_path, settings_.developer_name);
  cache_path = PlatformFileSystem::AppendPath(cache_path, settings_.app_name);
  PlatformFileSystem::MakeAllDirectories(cache_path);

  String log_path = PlatformFileSystem::AppendPath(cache_path, "ultralight.log");

  logger_.reset(new FileLogger(log_path));
  Platform::instance().set_logger(logger_.get());

  std::ostringstream info;
  info << "Writing log to: " << log_path.utf8().data() << std::endl;
  OutputDebugStringA(info.str().c_str());

  // Get module path
  String module_path = GetModulePath();

  String resource_path = PlatformFileSystem::AppendPath(module_path, "resources");
  
  config.resource_path = resource_path.utf16();
  config.cache_path = cache_path.utf16();
  config.device_scale = main_monitor_->scale();
  config.face_winding = kFaceWinding_Clockwise;
  Platform::instance().set_config(config);

  font_loader_.reset(new FontLoaderWin());
  Platform::instance().set_font_loader(font_loader_.get());

  // Replace forward slashes with backslashes for proper path
  // resolution on Windows
  std::wstring fs_str = settings.file_system_path.utf16().data();
  std::replace(fs_str.begin(), fs_str.end(), L'/', L'\\');

  String file_system_path = PlatformFileSystem::AppendPath(module_path,
    String16(fs_str.data(), fs_str.length()));

  file_system_.reset(new FileSystemWin(file_system_path.utf16().data()));
  Platform::instance().set_file_system(file_system_.get());

  clipboard_.reset(new ClipboardWin());
  Platform::instance().set_clipboard(clipboard_.get());

  if (!Platform::instance().config().use_gpu_renderer) {
    surface_factory_.reset(new DIBSurfaceFactory(GetDC(0)));
    Platform::instance().set_surface_factory(surface_factory_.get());
  }

  renderer_ = Renderer::Create();

  info.clear();
  info << "File system base directory resolved to: " << file_system_path.utf8().data();
  UL_LOG_INFO(info.str().c_str());
}

AppWin::~AppWin() {
  Platform::instance().set_gpu_driver(nullptr);
  gpu_driver_.reset();
  gpu_context_.reset();
}

void AppWin::OnClose() {
}

void AppWin::OnResize(uint32_t width, uint32_t height) {
  if (gpu_context_) {
    gpu_context_->Resize((int)width, (int)height);
  }
}

void AppWin::set_window(Ref<Window> window) {
  window_ = window;
  WindowWin* win = static_cast<WindowWin*>(window_.get());
  win->set_app_listener(this);

  if (!Platform::instance().config().use_gpu_renderer)
    return;

#if defined(DRIVER_D3D11)
  gpu_context_.reset(new GPUContextD3D11());
  
  if (!gpu_context_->Initialize(win->hwnd(), win->width(),
    win->height(), win->scale(), win->is_fullscreen(), true, true, 1)) {
    MessageBoxW(NULL, (LPCWSTR)L"Failed to initialize D3D11 context", (LPCWSTR)L"Notification", MB_OK);
    exit(-1);
  }

  gpu_driver_.reset(new GPUDriverD3D11(gpu_context_.get()));
#elif defined(DRIVER_D3D12)
  gpu_context_.reset(new GPUContextD3D12());
  WindowWin* win = static_cast<WindowWin*>(window_.get());
  if (!gpu_context_->Initialize(win->hwnd(), win->width(),
	  win->height(), win->scale(), win->is_fullscreen(), true, true, 1)) {
	  MessageBoxW(NULL, (LPCWSTR)L"Failed to initialize D3D12 context", (LPCWSTR)L"Notification", MB_OK);
	  exit(-1);
  }

  gpu_driver_.reset(new GPUDriverD3D12(gpu_context_.get()));
#endif
  Platform::instance().set_gpu_driver(gpu_driver_.get());
}

Monitor* AppWin::main_monitor() {
  return main_monitor_.get();
}

Ref<Renderer> AppWin::renderer() {
  return *renderer_.get();
}

void AppWin::Run() {
  if (!window_) {
    MessageBoxW(NULL, (LPCWSTR)L"Forgot to call App::set_window before App::Run", (LPCWSTR)L"Notification", MB_OK);
    exit(-1);
  }

  if (is_running_)
    return;

  std::chrono::milliseconds interval_ms(2);
  std::chrono::steady_clock::time_point next_paint = std::chrono::steady_clock::now();

  is_running_ = true;
  while (is_running_) {
    auto timeout_ns = next_paint - std::chrono::steady_clock::now();
    long long timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timeout_ns).count();
    DWORD timeout = timeout_ms <= 0 ? 0 : (DWORD)timeout_ms;
    DWORD result = (timeout
      ? MsgWaitForMultipleObjects(0, 0, TRUE, timeout, QS_ALLEVENTS)
      : WAIT_TIMEOUT);
    if (result == WAIT_TIMEOUT) {
      Update();
      WindowWin* win = static_cast<WindowWin*>(window_.get());
      if (win->NeedsRepaint())
        InvalidateRect(win->hwnd(), nullptr, false);
      next_paint = std::chrono::steady_clock::now() + interval_ms;
    }

    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        return;
      }

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
}

void AppWin::Quit() {
  is_running_ = false;
}

void AppWin::OnPaint() {
  if (!Platform::instance().config().use_gpu_renderer) {
    renderer_->Render();

    if (window_)
      static_cast<WindowWin*>(window_.get())->Draw();

    return;
  }

  if (!gpu_driver_)
    return;

  gpu_driver_->BeginSynchronize();
  renderer_->Render();
  gpu_driver_->EndSynchronize();

  if (gpu_driver_->HasCommandsPending()) {
    gpu_context_->BeginDrawing();
    gpu_driver_->DrawCommandList();
    if (window_)
	    static_cast<WindowWin*>(window_.get())->Draw();
    gpu_context_->PresentFrame();
    gpu_context_->EndDrawing();
    is_first_paint_ = false;
  }
  else if (window_needs_repaint_ && !is_first_paint_) {
    gpu_context_->BeginDrawing();
    if (window_)
      static_cast<WindowWin*>(window_.get())->Draw();
    gpu_context_->PresentFrame();
    gpu_context_->EndDrawing();
  }

  window_needs_repaint_ = false;
}

void AppWin::Update() {
  if (listener_)
    listener_->OnUpdate();

  renderer()->Update();
}

static App* g_app_instance = nullptr;

Ref<App> App::Create(Settings settings, Config config) {
  g_app_instance = new AppWin(settings, config);
  return AdoptRef(*g_app_instance);
}

App::~App() {
  g_app_instance = nullptr;
}

App* App::instance() {
  return g_app_instance;
}

}  // namespace ultralight
