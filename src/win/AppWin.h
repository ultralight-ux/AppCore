#pragma once
#include "ClipboardWin.h"
#include "FileLogger.h"
#include "RefCountedImpl.h"
#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <Windows.h>
#include <algorithm>
#include <chrono>
#include <memory>
#include <vector>

namespace ultralight {

#if defined(DRIVER_D3D11)
class GPUContextD3D11;
class GPUDriverD3D11;
#elif defined(DRIVER_D3D12)
class GPUContextD3D12;
class GPUDriverD3D12;
#endif
class WindowsUtil;
class MonitorWin;
class WindowWin;
class DIBSurfaceFactory;

class AppWin : public App, public RefCountedImpl<AppWin> {
public:
    // Inherited from App

    virtual const Settings& settings() const override { return settings_; }

    virtual void set_listener(AppListener* listener) override { listener_ = listener; }

    virtual AppListener* listener() override { return listener_; }

    virtual bool is_running() const override { return is_running_; }

    virtual Monitor* main_monitor() override;

    virtual RefPtr<Renderer> renderer() override;

    virtual void Run() override;

    virtual void Quit() override;

    REF_COUNTED_IMPL(AppWin);

protected:
    AppWin(Settings settings, Config config);
    virtual ~AppWin();
    void RefreshDisplay();
    void Update();

#if defined(DRIVER_D3D11)
    GPUContextD3D11* gpu_context();
    GPUDriverD3D11* gpu_driver();
#elif defined(DRIVER_D3D12)
#endif

    void AddWindow(WindowWin* window) { windows_.push_back(window); }

    void RemoveWindow(WindowWin* window)
    {
        windows_.erase(std::remove(windows_.begin(), windows_.end(), window), windows_.end());
    }

    friend class App;
    friend class WindowWin;

    DISALLOW_COPY_AND_ASSIGN(AppWin);

    Settings settings_;
    bool is_running_ = false;
    AppListener* listener_ = nullptr;
    std::vector<WindowWin*> windows_;
    RefPtr<Renderer> renderer_;
    std::unique_ptr<WindowsUtil> windows_util_;
    std::unique_ptr<MonitorWin> main_monitor_;
    std::unique_ptr<ClipboardWin> clipboard_;
#if defined(DRIVER_D3D11)
    std::unique_ptr<GPUContextD3D11> gpu_context_;
    std::unique_ptr<GPUDriverD3D11> gpu_driver_;
#elif defined(DRIVER_D3D12)
    std::unique_ptr<GPUContextD3D12> gpu_context_;
    std::unique_ptr<GPUDriverD3D12> gpu_driver_;
#endif
    std::unique_ptr<FileLogger> logger_;
    std::unique_ptr<DIBSurfaceFactory> surface_factory_;
    std::chrono::steady_clock::time_point last_statistics_update_;
};

} // namespace ultralight
