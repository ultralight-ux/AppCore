#pragma once
#include <AppCore/App.h>
#include <AppCore/Window.h>
#include "RefCountedImpl.h"
#include "MonitorMac.h"
#include "FileLogger.h"
#include <memory>
#import <QuartzCore/CAMetalLayer.h>

namespace ultralight {
    
class AppMac;
class GPUContextMetal;
class FileSystemMac;
class FontLoaderMac;
    
class AppMac : public App,
               public RefCountedImpl<AppMac>,
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

  REF_COUNTED_IMPL(AppMac);
                   
  void Update();
                   
  void OnPaint(CAMetalLayer* layer);

protected:
  AppMac(Settings settings, Config config);
  virtual ~AppMac();

  friend class App;
  
  DISALLOW_COPY_AND_ASSIGN(AppMac);

  bool is_running_ = false;
  Settings settings_;
  AppListener* listener_ = nullptr;
  RefPtr<Renderer> renderer_;
  RefPtr<Window> window_;
  MonitorMac main_monitor_;
  std::unique_ptr<GPUContextMetal> gpu_context_;
  std::unique_ptr<FileSystemMac> file_system_;
  std::unique_ptr<FontLoaderMac> font_loader_;
  std::unique_ptr<FileLogger> logger_;
};
    
    
}  // namespace ultralight
