#pragma once
#include <AppCore/App.h>
#include "gl/GPUContextGL.h"
#include <AppCore/Window.h>
#include "RefCountedImpl.h"
#include "MonitorGLFW.h"
#include <vector>
#include <memory>
#include <algorithm>

namespace ultralight {

class GPUContextGL;
class GPUDriverGL;
class FileLogger;
class ClipboardGLFW;
class WindowGLFW;

class AppGLFW : public App,
                public RefCountedImpl<AppGLFW> {
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

  REF_COUNTED_IMPL(AppGLFW);

protected:
  AppGLFW(Settings settings, Config config);
  virtual ~AppGLFW();

  void Update();

  GPUContextGL* gpu_context() { return gpu_context_.get(); }
  GPUDriverImpl* gpu_driver() { return gpu_context_->driver(); }

  void AddWindow(WindowGLFW* window) { windows_.push_back(window); }

  void RemoveWindow(WindowGLFW* window) {
    windows_.erase(std::remove(windows_.begin(), windows_.end(), window), windows_.end());
  }

  friend class App;
  friend class WindowGLFW;

  DISALLOW_COPY_AND_ASSIGN(AppGLFW);

  bool is_running_ = false;
  AppListener* listener_ = nullptr;
  std::vector<WindowGLFW*> windows_;
  Settings settings_;
  RefPtr<Renderer> renderer_;
  RefPtr<Window> window_;
  std::unique_ptr<MonitorGLFW> main_monitor_;
  std::unique_ptr<GPUContextGL> gpu_context_;
  std::unique_ptr<ClipboardGLFW> clipboard_;
  std::unique_ptr<FileLogger> logger_;
};

}  // namespace ultralight
