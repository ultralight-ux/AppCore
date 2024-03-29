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
#include <Security/SecTask.h>

@interface AppTimers : NSObject {
    dispatch_source_t updateTimer;
    dispatch_source_t refreshTimer;
    double updateFrequency;
    double refreshFrequency;
}
- (id)initWithUpdateFrequency:(double)updateFreq refreshFrequency:(double)refreshFreq;
- (void)startTimers;
- (void)stopTimers;
@end

@implementation AppTimers

- (id)initWithUpdateFrequency:(double)updateFreq refreshFrequency:(double)refreshFreq {
    self = [super init];
    if (self) {
        updateFrequency = updateFreq;
        refreshFrequency = refreshFreq;
        [self startTimers];
    }
    return self;
}

- (void)dealloc {
    [self stopTimers];
}

- (void)startTimers {
    // Update Timer
    updateTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
    if (updateTimer) {
        dispatch_source_set_timer(updateTimer, DISPATCH_TIME_NOW, (1.0 / updateFrequency) * NSEC_PER_SEC, 0);
        dispatch_source_set_event_handler(updateTimer, ^{
            static_cast<ultralight::AppMac*>(ultralight::App::instance())->Update();
        });
        dispatch_resume(updateTimer);
    }

    // Refresh Timer
    refreshTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
    if (refreshTimer) {
        dispatch_source_set_timer(refreshTimer, DISPATCH_TIME_NOW, (1.0 / refreshFrequency) * NSEC_PER_SEC, 0);
        dispatch_source_set_event_handler(refreshTimer, ^{
            static_cast<ultralight::AppMac*>(ultralight::App::instance())->Refresh();
        });
        dispatch_resume(refreshTimer);
    }
}

- (void)stopTimers {
    if (updateTimer) {
        dispatch_source_cancel(updateTimer);
        updateTimer = NULL;
    }
    if (refreshTimer) {
        dispatch_source_cancel(refreshTimer);
        refreshTimer = NULL;
    }
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

std::filesystem::path ToFileSystemPath(const __CFString* cfString) {
    if (cfString) {
        CFIndex maxSize = CFStringGetMaximumSizeForEncoding(CFStringGetLength(cfString), kCFStringEncodingUTF8) + 1;
        std::vector<char> cString(maxSize);
        if (CFStringGetCString(cfString, cString.data(), maxSize, kCFStringEncodingUTF8))
            return std::filesystem::path(cString.data());
    }

    return std::filesystem::path();
}

static std::filesystem::path GetSystemCachePath() {
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
  NSString* cacheDir = [paths objectAtIndex:0];
  return ToFileSystemPath((__bridge CFStringRef)cacheDir);
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
  
  std::filesystem::path cache_path = GetSystemCachePath();
  String cache_dirname = "com." + settings_.developer_name + "." +
    settings_.app_name;
  cache_path /= std::string(cache_dirname.utf8().data());
  if (!std::filesystem::create_directories(cache_path)) {
    std::cout << "Failed to create cache path: " << cache_path.string() << std::endl;
  }

  if (!Platform::instance().logger()) {
    std::filesystem::path log_path = cache_path / std::filesystem::path("ultralight.log");
    logger_.reset(new FileLogger(log_path.string().c_str()));
    Platform::instance().set_logger(logger_.get());
  }

  // Determine resources path
  String bundle_resource_path = GetBundleResourcePath();

  config.cache_path = cache_path.string().c_str();
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

  surface_factory_.reset(new ULTextureSurfaceFactory());
  Platform::instance().set_surface_factory(surface_factory_.get());
  
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

  // Initialize AppTimers with desired frequencies
  // Note: we display at twice the refresh rate to reduce temporal aliasing (see: nyquist rate)
  double updateRate = 500.0;
  double refreshRate = main_monitor()->refresh_rate() * 2.0;
  AppTimers* timers = [[AppTimers alloc] initWithUpdateFrequency:updateRate refreshFrequency:refreshRate];

  [NSApp run];

  // With ARC, just set the variable to nil to release it
  timers = nil;
  is_running_ = false;
}

void AppMac::Quit() {
  [NSApp terminate:nil];
}

void AppMac::Update() {
  if (listener_)
    listener_->OnUpdate();

  renderer()->Update();

  bool force_repaint = Platform::instance().config().force_repaint;

  for (auto i : windows_) {
    if (i->NeedsRepaint() || force_repaint)
      i->SetNeedsDisplay();
  }
}

void AppMac::Refresh() {
  App::instance()->renderer()->RefreshDisplay(main_monitor()->display_id());
  Update();
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
