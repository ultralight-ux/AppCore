#include "WindowMac.h"
#include <AppCore/Monitor.h>
#include "AppMac.h"
#import "Cocoa/Cocoa.h"
#import "ViewController.h"
#import "metal/GPUContextMetal.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/GPUDriver.h>

namespace ultralight {

WindowMac::WindowMac(Monitor* monitor, uint32_t width, uint32_t height,
  bool fullscreen, unsigned int window_flags) : monitor_(monitor), is_fullscreen_(fullscreen) {
  NSRect frame = NSMakeRect(0, 0, width, height);
  NSWindowStyleMask style = 0;
  if (window_flags & kWindowFlags_Borderless)
    style |= NSWindowStyleMaskBorderless;
  else
    style |= NSWindowStyleMaskClosable;
  if (window_flags & kWindowFlags_Titled)
    style |= NSWindowStyleMaskTitled;
  if (window_flags & kWindowFlags_Resizable)
    style |= NSWindowStyleMaskResizable;
  if (window_flags & kWindowFlags_Maximizable)
    style |= NSWindowStyleMaskMiniaturizable;

  window_  = [[NSWindow alloc] initWithContentRect:frame
                                         styleMask:style
                                             backing:NSBackingStoreBuffered
                                               defer:NO];
  
  delegate_ = [[WindowDelegate alloc] init];
  [delegate_ initWithWindow:this];
  [window_ setDelegate:delegate_];
  
  [window_ center];
  [window_ setBackgroundColor:[NSColor whiteColor]];
  [window_ makeKeyAndOrderFront:NSApp];
  [window_ setOrderedIndex:0];
  [NSApp activateIgnoringOtherApps:YES];
  
  controller_ = [[ViewController alloc] init];
  [controller_ initWithWindow:this frame:frame];
  [window_ setContentViewController:controller_];
    
  // Move app to foreground
  ProcessSerialNumber psn = {0, kCurrentProcess};
  TransformProcessType(&psn, kProcessTransformToForegroundApplication);

  SetWindowScale(scale());
  
  auto app = static_cast<AppMac*>(App::instance());
  GPUContextMetal* gpu_context = nullptr;
  if (app) {
    app->AddWindow(this);
    
    gpu_context = app->gpu_context();
  }
  
  auto driver = ultralight::Platform::instance().gpu_driver();
  render_buffer_id_ = driver? driver->NextRenderBufferId() : 0;
  current_drawable_ = [layer() nextDrawable];
  
  if (gpu_context)
    gpu_context->AddWindow(this);
}

WindowMac::~WindowMac() {
  auto app = static_cast<AppMac*>(App::instance());
  if (app) {
    app->RemoveWindow(this);
    auto gpu_context = app->gpu_context();
    if (gpu_context)
      gpu_context->RemoveWindow(this);
  }
  
  [window_ setContentViewController:nil];
  [window_ setDelegate:nil];
}

uint32_t WindowMac::width() const {
  return (uint32_t)controller_.metalView.metalLayer.drawableSize.width;
}

uint32_t WindowMac::height() const {
  return (uint32_t)controller_.metalView.metalLayer.drawableSize.height;
}

double WindowMac::scale() const {
  return monitor_->scale();
}

void WindowMac::SetTitle(const char* title) {
  window_.title = [NSString stringWithUTF8String:title];
}

void WindowMac::SetCursor(ultralight::Cursor cursor) {
  switch (cursor) {
    case ultralight::kCursor_Hand: {
      [[NSCursor pointingHandCursor] set];
      break;
    }
    case ultralight::kCursor_Pointer: {
      [[NSCursor arrowCursor] set];
      break;
    }
    case ultralight::kCursor_IBeam: {
      [[NSCursor IBeamCursor] set];
      break;
    }
  };
}

void WindowMac::Close() {
  [window_ close];
}

void* WindowMac::native_handle() const {
  return (__bridge void *)window_;
}

void WindowMac::OnClose() {
  if (listener_)
    listener_->OnClose(this);
  if (app_listener_)
    app_listener_->OnClose(this);
}

void WindowMac::OnResize(uint32_t width, uint32_t height) {
  if (app_listener_)
    app_listener_->OnResize(this, width, height);
  if (listener_)
    listener_->OnResize(this, width, height);
}
    
void WindowMac::SetNeedsDisplay() {
  controller_.metalView.needsDisplay = true;
}

void WindowMac::OnPaint(CAMetalLayer* layer) {
  auto gpu_context = static_cast<AppMac*>(App::instance())->gpu_context();
  if (!gpu_context)
    return;

  if (!NeedsRepaint())
    return;
  
  gpu_context->driver()->BeginSynchronize();
  OverlayManager::Render();
  gpu_context->driver()->EndSynchronize();

  if (gpu_context->driver()->HasCommandsPending() || OverlayManager::NeedsRepaint()) {
    gpu_context->BeginDrawing();
    gpu_context->driver()->DrawCommandList();
    OverlayManager::Paint();
    gpu_context->EndDrawing();
    [current_drawable_ present];
    current_drawable_ = [layer nextDrawable];
  }
}
    
CAMetalLayer* WindowMac::layer() {
  return controller_.metalView.metalLayer;
}

Ref<Window> Window::Create(Monitor* monitor, uint32_t width, uint32_t height,
  bool fullscreen, unsigned int window_flags) {
  return AdoptRef(*new WindowMac(monitor, width, height, fullscreen, window_flags));
}

Window::~Window() {}

}  // namespace ultralight

