#pragma once
#include <AppCore/App.h>
#include <AppCore/Window.h>
#include "RefCountedImpl.h"
#include "FontLoaderWin.h"
#include "FileSystemWin.h"
#include <vector>
#include <memory>

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

class AppWin : public App,
               public RefCountedImpl<AppWin>,
               public WindowListener {
public:
  // Inherited from WindowListener

  virtual void OnClose() override;

  virtual void OnResize(uint32_t width, uint32_t height) override;

  // Inherited from App

  virtual const Settings& settings() const override { return settings_; }

  virtual void set_listener(AppListener* listener) override { listener_ = listener; }

  virtual void set_window(Ref<Window> window) override;

  virtual RefPtr<Window> window() override { return window_; }

  virtual AppListener* listener() override { return listener_; }

  virtual bool is_running() const override { return is_running_; }

  virtual Monitor* main_monitor() override;

  virtual Ref<Renderer> renderer() override;

  virtual void Run() override;

  virtual void Quit() override;

  REF_COUNTED_IMPL(AppWin);

  void OnPaint();

  void InvalidateWindow() { window_needs_repaint_ = true; }

protected:
  AppWin(Settings settings, Config config);
  virtual ~AppWin();
  void Update();

  friend class App;
  
  DISALLOW_COPY_AND_ASSIGN(AppWin);

  Settings settings_;
  bool is_running_ = false;
  bool is_first_paint_ = true;
  bool window_needs_repaint_ = false;
  AppListener* listener_ = nullptr;
  RefPtr<Renderer> renderer_;
  RefPtr<Window> window_;
  std::unique_ptr<WindowsUtil> windows_util_;
  std::unique_ptr<MonitorWin> main_monitor_;
  std::unique_ptr<FontLoaderWin> font_loader_;
  std::unique_ptr<FileSystemWin> file_system_;
#if defined(DRIVER_D3D11)
  std::unique_ptr<GPUContextD3D11> gpu_context_;
  std::unique_ptr<GPUDriverD3D11> gpu_driver_;
#elif defined(DRIVER_D3D12)
  std::unique_ptr<GPUContextD3D12> gpu_context_;
  std::unique_ptr<GPUDriverD3D12> gpu_driver_;
#endif
};

}  // namespace ultralight
