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
#include <Shlwapi.h>
#include "WindowsUtil.h"
#include "MonitorWin.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace ultralight {

AppWin::AppWin(Settings settings, Config config) : settings_(settings) {
  windows_util_.reset(new WindowsUtil());
  windows_util_->EnableDPIAwareness();

  main_monitor_.reset(new MonitorWin(windows_util_.get()));

  HMODULE hModule = GetModuleHandleW(NULL);
  WCHAR module_path[MAX_PATH];
  GetModuleFileNameW(hModule, module_path, MAX_PATH);
  PathRemoveFileSpecW(module_path);

  WCHAR log_path[MAX_PATH];
  PathCombineW(log_path, module_path, L"ultralight.log");

  logger_.reset(new FileLogger(String16(log_path, lstrlenW(log_path))));
  Platform::instance().set_logger(logger_.get());

  WCHAR resource_path[MAX_PATH];
  PathCombineW(resource_path, module_path, L"resources");
  WCHAR cache_path[MAX_PATH];
  PathCombineW(cache_path, module_path, L"cache");

  config.resource_path = String16(resource_path, lstrlenW(resource_path));
  config.cache_path = String16(cache_path, lstrlenW(cache_path));
  config.device_scale = main_monitor_->scale();
  config.face_winding = kFaceWinding_Clockwise;
  Platform::instance().set_config(config);

  font_loader_.reset(new FontLoaderWin());
  Platform::instance().set_font_loader(font_loader_.get());

  std::wstring fs_str = settings.file_system_path.utf16().data();
  std::replace(fs_str.begin(), fs_str.end(), L'/', L'\\');

  WCHAR file_system_path[MAX_PATH];
  PathCombineW(file_system_path, module_path, fs_str.data());

  file_system_.reset(new FileSystemWin(file_system_path));
  Platform::instance().set_file_system(file_system_.get());

  renderer_ = Renderer::Create();

  std::ostringstream info;
  info << "File system base directory resolved to: " << String(file_system_path, lstrlenW(file_system_path)).utf8().data();
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
  if (gpu_context_)
    gpu_context_->Resize((int)width, (int)height);
}

void AppWin::set_window(Ref<Window> window) {
  window_ = window;
#if defined(DRIVER_D3D11)
  gpu_context_.reset(new GPUContextD3D11());
  WindowWin* win = static_cast<WindowWin*>(window_.get());
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
  win->set_app_listener(this);
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

  MSG msg = { 0 };
  is_running_ = true;
  while (WM_QUIT != msg.message && is_running_) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else {
      OnPaint();

      // Sleep a tiny bit to reduce CPU usage
      Sleep(1);
    }
  }
}

void AppWin::Quit() {
  is_running_ = false;
}

void AppWin::OnPaint() {
  Update();
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
