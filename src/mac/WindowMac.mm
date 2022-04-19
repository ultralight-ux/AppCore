#include "WindowMac.h"
#include <AppCore/Monitor.h>
#include "AppMac.h"
#import "Cocoa/Cocoa.h"
#import "ViewController.h"
#import "metal/GPUContextMetal.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/GPUDriver.h>

namespace ultralight {

static CGPoint g_cascade_point = { 0.0f, 0.0f };

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
  g_cascade_point =
            NSPointToCGPoint([window_ cascadeTopLeftFromPoint:
                              NSPointFromCGPoint(g_cascade_point)]);
  [window_ setBackgroundColor:[NSColor whiteColor]];
  if (!(window_flags & kWindowFlags_Hidden)) {
    [window_ makeKeyAndOrderFront:NSApp];
    [NSApp activateIgnoringOtherApps:YES];
  }
  
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

uint32_t WindowMac::screen_width() const {
  return PixelsToScreen(width());
}

uint32_t WindowMac::width() const {
  return (uint32_t)controller_.metalView.metalLayer.drawableSize.width;
}

uint32_t WindowMac::screen_height() const {
  return PixelsToScreen(height());
}

uint32_t WindowMac::height() const {
  return (uint32_t)controller_.metalView.metalLayer.drawableSize.height;
}

double WindowMac::scale() const {
  return monitor_->scale();
}

// Transforms the specified y-coordinate between the CG display and NS screen
// coordinate systems
static float transformY(float y)
{
    return CGDisplayBounds(CGMainDisplayID()).size.height - y;
}

void WindowMac::MoveTo(int x, int y) {
    const NSRect contentRect = [controller_.metalView frame];
    const NSRect dummyRect = NSMakeRect(x, transformY(y + contentRect.size.height), 0, 0);
    const NSRect frameRect = [window_ frameRectForContentRect:dummyRect];
    [window_ setFrameOrigin:frameRect.origin];
}

void WindowMac::MoveToCenter() {
    [window_ center];
}

int WindowMac::x() const {
    const NSRect contentRect =
        [window_ contentRectForFrameRect:[window_ frame]];

    return contentRect.origin.x;
}

int WindowMac::y() const {
    const NSRect contentRect =
        [window_ contentRectForFrameRect:[window_ frame]];

    return transformY(contentRect.origin.y + contentRect.size.height);
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

void WindowMac::Show() {
  [window_ orderFront:nil];
  [window_ makeKeyAndOrderFront:NSApp];
  [NSApp activateIgnoringOtherApps:YES];
}

void WindowMac::Hide() {
  [window_ orderOut:nil];
}

bool WindowMac::is_visible() const {
  return [window_ isVisible];
}

void WindowMac::Close() {
  [window_ close];
}

void* WindowMac::native_handle() const {
  return (__bridge void *)window_;
}

void WindowMac::FireKeyEvent(const ultralight::KeyEvent& evt) {
  if (listener())
    if (!listener()->OnKeyEvent(evt))
      return;

  OverlayManager::FireKeyEvent(evt);
}

void WindowMac::FireMouseEvent(const ultralight::MouseEvent& evt) {
  if (listener())
    if (!listener()->OnMouseEvent(evt))
      return;

  OverlayManager::FireMouseEvent(evt);
}

void WindowMac::FireScrollEvent(const ultralight::ScrollEvent& evt) {
  if (listener())
    if (!listener()->OnScrollEvent(evt))
      return;

  OverlayManager::FireScrollEvent(evt);
}

void WindowMac::OnClose() {
  // Keep window alive in case user-callbacks release our reference.
  RefPtr<WindowMac> retain(this);

  if (listener_)
    listener_->OnClose(this);
  if (app_listener_)
    app_listener_->OnClose(this);
}

void WindowMac::OnResize(uint32_t width, uint32_t height) {
  // Keep window alive in case user-callbacks release our reference.
  RefPtr<WindowMac> retain(this);

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

RefPtr<Window> Window::Create(Monitor* monitor, uint32_t width, uint32_t height,
  bool fullscreen, unsigned int window_flags) {
  return AdoptRef(*static_cast<Window*>(new WindowMac(monitor, width, height, fullscreen, window_flags)));
}

Window::~Window() {}

}  // namespace ultralight

