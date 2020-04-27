#pragma once
#include <AppCore/App.h>
#include <AppCore/Window.h>
#include "RefCountedImpl.h"
#include "MonitorGLFW.h"
#include <vector>
#include <memory>

namespace ultralight {

class GPUContextGL;
class GPUDriverGL;
class FileSystemBasic;
class FileLogger;
class FontLoaderLinux;

class AppGLFW : public App,
                public RefCountedImpl<AppGLFW>,
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

  REF_COUNTED_IMPL(AppGLFW);

protected:
  AppGLFW(Settings settings, Config config);
  virtual ~AppGLFW();
  void Update();

  friend class App;
  
  DISALLOW_COPY_AND_ASSIGN(AppGLFW);

  bool is_running_ = false;
  AppListener* listener_ = nullptr;
  Settings settings_;
  RefPtr<Renderer> renderer_;
  RefPtr<Window> window_;
  std::unique_ptr<MonitorGLFW> main_monitor_;
  std::unique_ptr<GPUContextGL> gpu_context_;
  std::unique_ptr<FileSystemBasic> file_system_;
  std::unique_ptr<FontLoaderLinux> font_loader_;
  std::unique_ptr<FileLogger> logger_;
};

}  // namespace ultralight
