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
#include <Ultralight/private/tracy/Tracy.hpp>
#include <AppCore/Platform.h>
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

  std::ostringstream info;

  if (!Platform::instance().logger()) {
    String log_path = PlatformFileSystem::AppendPath(cache_path, "ultralight.log");

    logger_.reset(new FileLogger(log_path));
    Platform::instance().set_logger(logger_.get());

    
    info << "Writing log to: " << log_path.utf8().data() << std::endl;
    OutputDebugStringA(info.str().c_str());
  }

  // Get module path
  String module_path = GetModulePath();

  config.cache_path = cache_path.utf16();
  config.face_winding = FaceWinding::Clockwise;
  Platform::instance().set_config(config);

  if (!Platform::instance().font_loader()) {
    Platform::instance().set_font_loader(GetPlatformFontLoader());
  }

  if (!Platform::instance().file_system()) {
    // Replace forward slashes with backslashes for proper path
    // resolution on Windows
    std::wstring fs_str = settings.file_system_path.utf16().data();
    std::replace(fs_str.begin(), fs_str.end(), L'/', L'\\');

    String file_system_path = PlatformFileSystem::AppendPath(module_path,
      String16(fs_str.data(), fs_str.length()));

    Platform::instance().set_file_system(GetPlatformFileSystem(file_system_path));

    info.clear();
    info << "File system base directory resolved to: " << file_system_path.utf8().data();
    UL_LOG_INFO(info.str().c_str());
  }

  clipboard_.reset(new ClipboardWin());
  Platform::instance().set_clipboard(clipboard_.get());

  if (settings_.force_cpu_renderer) {
    surface_factory_.reset(new DIBSurfaceFactory(GetDC(0)));
    Platform::instance().set_surface_factory(surface_factory_.get());
  } else {
#if defined(DRIVER_D3D11)
    gpu_context_.reset(new GPUContextD3D11());
    if (gpu_context_->device()) {
      gpu_driver_.reset(new GPUDriverD3D11(gpu_context_.get()));
      Platform::instance().set_gpu_driver(gpu_driver_.get());
    } else {
      gpu_context_.reset();
    }
#elif defined(DRIVER_D3D12)
    // TODO
#endif
  }

  renderer_ = Renderer::Create();
}

AppWin::~AppWin() {
  Platform::instance().set_gpu_driver(nullptr);
  Platform::instance().set_clipboard(nullptr);
  Platform::instance().set_file_system(nullptr);
  Platform::instance().set_font_loader(nullptr);
  Platform::instance().set_logger(nullptr);
  Platform::instance().set_surface_factory(nullptr);
  gpu_driver_.reset();
  gpu_context_.reset();
}

Monitor* AppWin::main_monitor() {
  return main_monitor_.get();
}

RefPtr<Renderer> AppWin::renderer() {
  return renderer_;
}

void AppWin::Run() {
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
      for (auto window : windows_) {
        if (window->NeedsRepaint())
          InvalidateRect(window->hwnd(), nullptr, false);
      }
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

void AppWin::Update() {
  if (listener_)
    listener_->OnUpdate();

  const char* frame_mark_update = "Update";

  FrameMarkStart(frame_mark_update);
  renderer()->Update();
  FrameMarkEnd(frame_mark_update);
}

GPUContextD3D11* AppWin::gpu_context() { return gpu_context_.get(); }

GPUDriverD3D11* AppWin::gpu_driver() { return gpu_driver_.get(); }

static App* g_app_instance = nullptr;

RefPtr<App> App::Create(Settings settings, Config config) {
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
