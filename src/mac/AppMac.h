#pragma once
#include <AppCore/App.h>
#include <AppCore/Window.h>
#include "RefCountedImpl.h"
#include "MonitorMac.h"
#include "FileLogger.h"
#include <memory>
#include <vector>
#import <QuartzCore/CAMetalLayer.h>

namespace ultralight {
    
class AppMac;
class GPUContextMetal;
class ClipboardMac;
class WindowMac;
    
class AppMac : public App,
               public RefCountedImpl<AppMac> {
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

  REF_COUNTED_IMPL(AppMac);
                   
  void Update();
  
  GPUContextMetal* gpu_context();
  
  void AddWindow(WindowMac* window) { windows_.push_back(window); }

  void RemoveWindow(WindowMac* window) {
    windows_.erase(std::remove(windows_.begin(), windows_.end(), window), windows_.end());
  }

protected:
  AppMac(Settings settings, Config config);
  virtual ~AppMac();

  friend class App;
  
  DISALLOW_COPY_AND_ASSIGN(AppMac);

  bool is_running_ = false;
  Settings settings_;
  AppListener* listener_ = nullptr;
  RefPtr<Renderer> renderer_;
  MonitorMac main_monitor_;
  std::unique_ptr<GPUContextMetal> gpu_context_;
  std::unique_ptr<ClipboardMac> clipboard_;
  std::unique_ptr<FileLogger> logger_;
  std::vector<WindowMac*> windows_;
};
    
    
}  // namespace ultralight
