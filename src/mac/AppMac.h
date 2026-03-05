#pragma once
#include "AppImpl.h"
#include <AppCore/Window.h>
#include "RefCountedImpl.h"
#include "MonitorMac.h"
#include "ULTextureSurface.h"
#include <memory>
#include <vector>
#import <QuartzCore/CAMetalLayer.h>

namespace ultralight {

class AppMac;
class GPUContextMetal;
class ClipboardMac;
class WindowMac;

class AppMac : public AppImpl,
               public RefCountedImpl<AppMac> {
public:
  // Inherited from App (platform-specific only)

  virtual Monitor* main_monitor() override;

  virtual void Run() override;

  virtual void Quit() override;

  REF_COUNTED_IMPL(AppMac);

  void Update();

  void Refresh();

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

  // Platform-specific members only (shared members are in AppImpl)
  MonitorMac main_monitor_;
  std::unique_ptr<GPUContextMetal> gpu_context_;
  std::unique_ptr<ClipboardMac> clipboard_;
  std::unique_ptr<ULTextureSurfaceFactory> surface_factory_;
  std::vector<WindowMac*> windows_;
};


}  // namespace ultralight
