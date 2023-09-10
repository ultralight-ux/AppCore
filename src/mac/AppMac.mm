#include "AppMac.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>
#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"
#import "WindowMac.h"
#import "metal/GPUContextMetal.h"
#import "metal/GPUDriverMetal.h"
#include <AppCore/Platform.h>
#include "ClipboardMac.h"
#include <vector>
#include <CoreFoundation/CFString.h>
#include <iostream>
#include <Ultralight/private/util/Debug.h>
#include <Ultralight/private/PlatformFileSystem.h>
#import <MetalKit/MetalKit.h>
#include <filesystem>

@interface UpdateTimer : NSObject
@property NSTimer *timer;
- (id)init;
- (void)onTick:(NSTimer *)aTimer;
@end

// Run update timer at 120 FPS
@implementation UpdateTimer
- (id)init {
  id newInstance = [super init];
  if (newInstance) {
    _timer = [NSTimer scheduledTimerWithTimeInterval:(1.0/120.0)
                                              target:self
                                            selector:@selector(onTick:)
                                            userInfo:nil
                                             repeats:YES];
  }
  
  return newInstance;
}

-(void)onTick:(NSTimer *)aTimer {
  static_cast<ultralight::AppMac*>(ultralight::App::instance())->Update();
}
@end

namespace ultralight {

static String16 ToString16(CFStringRef str) {
    if (!str)
        return String16();
    CFIndex size = CFStringGetLength(str);
    std::vector<Char16> buffer(size);
    CFStringGetCharacters(str, CFRangeMake(0, size), (UniChar*)buffer.data());
    return String16(buffer.data(), size);
}

static String16 GetSystemCachePath() {
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
  NSString* cacheDir = [paths objectAtIndex:0];
  return ToString16((__bridge CFStringRef)cacheDir);
}

static String16 GetBundleResourcePath() {
  return ToString16((__bridge CFStringRef)[[NSBundle mainBundle] resourcePath]);
}

AppMac::AppMac(Settings settings, Config config) : settings_(settings) {
  [NSApplication sharedApplication];
  
  AppDelegate *appDelegate = [[AppDelegate alloc] init];
  [NSApp setDelegate:appDelegate];

  // Generate cache path
  // TODO: Handle cache path (macOS has new rules for writable cache dirs)
  /*
  String cache_path = GetSystemCachePath();
  String cache_dirname = "com." + settings_.developer_name + "." +
    settings_.app_name;
  cache_path = PlatformFileSystem::AppendPath(cache_path, cache_dirname);
  PlatformFileSystem::MakeAllDirectories(cache_path);

  if (!Platform::instance().logger()) {
    std::string cache_path_str = cache_path.utf8().data();
    std::filesystem::path log_path = cache_path_str / std::filesystem::path("ultralight.log");
    
    logger_.reset(new FileLogger(log_path.string().c_str()));
    Platform::instance().set_logger(logger_.get());
  }
  */

  // Determine resources path
  String bundle_resource_path = GetBundleResourcePath();

  //config.cache_path = cache_path.utf16();
  config.face_winding = FaceWinding::Clockwise;
  //config.force_repaint = true;
  Platform::instance().set_config(config);

  if (!Platform::instance().file_system()) {
    // Determine file system path
    std::string fs_str = settings.file_system_path.utf8().data();
    std::string bundle_resource_path_str = bundle_resource_path.utf8().data();
    std::filesystem::path file_system_path = bundle_resource_path_str / std::filesystem::path(fs_str);

    Platform::instance().set_file_system(GetPlatformFileSystem(file_system_path.string().c_str()));
    
    //std::ostringstream info;
    //info << "File system base directory resolved to: " <<
    //  file_system_path.utf8().data();
    //UL_LOG_INFO(info.str().c_str());
  }

  if (!Platform::instance().font_loader()) {
    Platform::instance().set_font_loader(GetPlatformFontLoader());
  }
  
  clipboard_.reset(new ClipboardMac());
  Platform::instance().set_clipboard(clipboard_.get());
  
  renderer_ = Renderer::Create();
}

AppMac::~AppMac() {
}

Monitor* AppMac::main_monitor() {
  return &main_monitor_;
}

RefPtr<Renderer> AppMac::renderer() {
  return renderer_;
}

void AppMac::Run() {
  if (is_running_)
    return;

  is_running_ = true;
  UpdateTimer* timer = [[UpdateTimer alloc] init];
  [NSApp run];
  timer = nullptr;
  is_running_ = false;
}

void AppMac::Quit() {
  [NSApp terminate:nil];
}

void AppMac::Update() {
  if (listener_)
    listener_->OnUpdate();

  renderer()->Update();
  
  for (auto i : windows_) {
    if (i->NeedsRepaint())
      i->SetNeedsDisplay();
  }
}

GPUContextMetal* AppMac::gpu_context() {
  if (!gpu_context_) {
    // TODO, we need to handle settings.force_cpu_renderer
    gpu_context_.reset(new GPUContextMetal(MTLCreateSystemDefaultDevice(), true, true));
    Platform::instance().set_gpu_driver(gpu_context_->driver());
  }

  return gpu_context_.get();
}

static App* g_app_instance = nullptr;

RefPtr<App> App::Create(Settings settings, Config config) {
  g_app_instance = (App*)new AppMac(settings, config);
  return AdoptRef(*g_app_instance);
}

App::~App() {
  g_app_instance = nullptr;
}

App* App::instance() {
  return g_app_instance;
}

}  // namespace ultralight
