#include "AppWin.h"
#include "WindowWin.h"
#include <Windows.h>
#if defined(DRIVER_D3D11)
#include "d3d11/GPUContextD3D11.h"
#include "d3d11/GPUDriverD3D11.h"
#elif defined(DRIVER_D3D12)
#include "d3d12/GPUContextD3D12.h"
#include "d3d12/GPUDriverD3D12.h"
#endif
#include "DIBSurface.h"
#include "MonitorWin.h"
#include "WindowsUtil.h"
#include <AppCore/Platform.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <Ultralight/platform/Config.h>
#include <Ultralight/platform/Logger.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/private/PlatformFileSystem.h>
#include <Ultralight/private/tracy/Tracy.hpp>
#include <Ultralight/private/util/Debug.h>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace ultralight {

static String GetModulePath()
{
    HMODULE hModule = GetModuleHandleW(NULL);
    WCHAR module_path[MAX_PATH];
    GetModuleFileNameW(hModule, module_path, MAX_PATH);
    PathRemoveFileSpecW(module_path);
    return String16(module_path, lstrlenW(module_path));
}

static String GetRoamingAppDataPath()
{
    PWSTR appDataPath = NULL;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, NULL, NULL, &appDataPath);
    String result = String16(appDataPath, lstrlenW(appDataPath));
    ::CoTaskMemFree(static_cast<void*>(appDataPath));
    return result;
}

inline std::filesystem::path ToFSPath(const String& str) { return str.utf8().data(); }

AppWin::AppWin(Settings settings, Config config)
    : settings_(settings)
{
    windows_util_.reset(new WindowsUtil());
    windows_util_->EnableDPIAwareness();

    main_monitor_.reset(new MonitorWin(windows_util_.get()));

    // Generate cache path
    String cache_path16 = GetRoamingAppDataPath();
    std::filesystem::path cache_path = ToFSPath(GetRoamingAppDataPath());
    cache_path /= ToFSPath(settings_.developer_name);
    cache_path /= ToFSPath(settings_.app_name);
    std::filesystem::create_directories(cache_path);

    std::ostringstream info;

    if (!Platform::instance().logger()) {
        std::filesystem::path log_path = cache_path / std::filesystem::path("ultralight.log");

        logger_.reset(new FileLogger(log_path.string().c_str()));
        Platform::instance().set_logger(logger_.get());

        info << "Writing log to: " << log_path.string().c_str() << std::endl;
        OutputDebugStringA(info.str().c_str());
    }

    // Get module path
    std::filesystem::path module_path = ToFSPath(GetModulePath());

    config.cache_path = cache_path.string().c_str();
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

        std::filesystem::path file_system_path = module_path / std::filesystem::path(fs_str);

        Platform::instance().set_file_system(GetPlatformFileSystem(file_system_path.string().c_str()));

        info.clear();
        info << "File system base directory resolved to: " << file_system_path.string().c_str();
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

AppWin::~AppWin()
{
    Platform::instance().set_gpu_driver(nullptr);
    Platform::instance().set_clipboard(nullptr);
    Platform::instance().set_file_system(nullptr);
    Platform::instance().set_font_loader(nullptr);
    Platform::instance().set_logger(nullptr);
    Platform::instance().set_surface_factory(nullptr);
    gpu_driver_.reset();
    gpu_context_.reset();
    WindowWin::CleanupWindowClass();
}

Monitor* AppWin::main_monitor() { return main_monitor_.get(); }

RefPtr<Renderer> AppWin::renderer() { return renderer_; }

void AppWin::Run()
{
    if (is_running_)
        return;

    // Initialize high resolution timer
    timeBeginPeriod(1); // Request the system to use a 1ms timer resolution

    is_running_ = true;

    // Use a special run-loop that doesn't yield to the OS when we are forcing repaints to see how
    // fast we can go (for benchmarking and profiling the renderer)
    if (Platform::instance().config().force_repaint) {
        while (is_running_) {

            RefreshDisplay();
            Update();

            // Process window messages to ensure responsiveness
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    is_running_ = false;
                    break; // Exit the loop if the WM_QUIT message is received
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        timeEndPeriod(1); // Reset the system timer resolution
        return;
    }

    HANDLE update_timer = CreateWaitableTimer(NULL, FALSE, NULL);
    HANDLE display_timer = CreateWaitableTimer(NULL, FALSE, NULL);

    auto setupTimer = [](HANDLE& timerHandle, const LONG targetFps) {
        LARGE_INTEGER li;
        const LONG timerPeriodMs = 1000 / targetFps;
        li.QuadPart = -10000LL * timerPeriodMs; // Convert to 100-nanosecond intervals
        bool result = SetWaitableTimer(timerHandle, &li, timerPeriodMs, NULL, NULL, FALSE);
        UL_DCHECK(result);
    };

    // Update JavaScript and timers at 500hz to keep main loop responsive
    setupTimer(update_timer, 500);

    // We display at twice the refresh rate to reduce temporal aliasing (see: nyquist rate)
    setupTimer(display_timer, main_monitor()->refresh_rate() * 2);

    HANDLE timers[] = { update_timer, display_timer };

    while (is_running_) {
        // Wait for the timer or a message, whichever comes first
        DWORD result = MsgWaitForMultipleObjects(2, timers, FALSE, INFINITE, QS_ALLEVENTS);

        if (result == WAIT_OBJECT_0) {
            Update();
        } else if (result == WAIT_OBJECT_0 + 1) {
            RefreshDisplay();
            Update();
        } else if (result == WAIT_OBJECT_0 + 2) {
            // Process window messages to ensure responsiveness
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    is_running_ = false;
                    break; // Exit the loop if the WM_QUIT message is received
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    // Cleanup
    CloseHandle(update_timer);
    CloseHandle(display_timer);
    timeEndPeriod(1); // Reset the system timer resolution
}

void AppWin::Quit() { is_running_ = false; }

void AppWin::RefreshDisplay()
{
    renderer()->RefreshDisplay(main_monitor_->display_id());
}

void AppWin::Update()
{
    if (listener_)
        listener_->OnUpdate();

    const char* frame_mark_update = "Update";

    FrameMarkStart(frame_mark_update);
    renderer()->Update();
    FrameMarkEnd(frame_mark_update);

    // if (main_monitor_->needs_refresh()) {
    // main_monitor_->set_needs_refresh(false);
    // renderer()->RefreshDisplay(main_monitor_->display_id());
    //}

    bool needs_stat_update = false;
    auto now = std::chrono::steady_clock::now();
    auto time_since_last_statistics_update = now - last_statistics_update_;
    if (time_since_last_statistics_update > std::chrono::seconds(1)) {
        needs_stat_update = true;
        last_statistics_update_ = now;
    }

    bool force_repaint = Platform::instance().config().force_repaint;

    for (auto window : windows_) {
        if (force_repaint || window->NeedsRepaint())
            InvalidateRect(window->hwnd(), nullptr, false);
        if (needs_stat_update)
            window->UpdateTitleWithStatistics();
    }
}

GPUContextD3D11* AppWin::gpu_context() { return gpu_context_.get(); }

GPUDriverD3D11* AppWin::gpu_driver() { return gpu_driver_.get(); }

static App* g_app_instance = nullptr;

RefPtr<App> App::Create(Settings settings, Config config)
{
    g_app_instance = new AppWin(settings, config);
    return AdoptRef(*g_app_instance);
}

App::~App() { g_app_instance = nullptr; }

App* App::instance() { return g_app_instance; }

} // namespace ultralight
