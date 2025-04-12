#include "WindowMac.h"
#include <AppCore/Monitor.h>
#include <AppCore/Overlay.h>
#include <AppCore/App.h>
#include "AppMac.h"
#include <sstream>
#import "Cocoa/Cocoa.h"
#import "ViewController.h"
#import "metal/GPUContextMetal.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/GPUDriver.h>
#include <Ultralight/private/tracy/Tracy.hpp>
#import "IOSurfaceView.h"
#include "IOSurfaceMac.h"

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

  // Initialize window scale
  window_scale_ = scale();
  SetWindowScale(window_scale_);
  
  auto app = static_cast<AppMac*>(App::instance());
  GPUContextMetal* gpu_context = nullptr;
  if (app) {
    app->AddWindow(this);
    
    gpu_context = app->gpu_context();
  }
  
  if (platform_always_uses_cpu_renderer()) {
    // For CPU rendering, we don't need a Metal drawable
    render_buffer_id_ = 0;
    current_drawable_ = nil;
  } else if (gpu_context) {
    // For GPU rendering, initialize Metal resources
    auto driver = ultralight::Platform::instance().gpu_driver();
    render_buffer_id_ = driver ? driver->NextRenderBufferId() : 0;
    current_drawable_ = [layer() nextDrawable];
    set_drawable_needs_clear(true);
    
    gpu_context->AddWindow(this);
  }
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
  if (platform_always_uses_cpu_renderer()) {
    // For CPU rendering path, get width from the controller's view
    return (uint32_t)(controller_.view.frame.size.width * window_scale_);
  } else {
    // For GPU rendering path, use the MTLView's metalLayer
    return (uint32_t)controller_.metalView.metalLayer.drawableSize.width;
  }
}

uint32_t WindowMac::screen_height() const {
  return PixelsToScreen(height());
}

uint32_t WindowMac::height() const {
  if (platform_always_uses_cpu_renderer()) {
    // For CPU rendering path, get height from the controller's view
    return (uint32_t)(controller_.view.frame.size.height * window_scale_);
  } else {
    // For GPU rendering path, use the MTLView's metalLayer
    return (uint32_t)controller_.metalView.metalLayer.drawableSize.height;
  }
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
  base_title_ = title;

  std::string titleStr = base_title_;

  if (frame_stats_enabled_) {
    if (titleStr.length() > 24) {
      titleStr = titleStr.substr(0, 24) + "..."; // Truncate and append ellipsis
    }

  if (!statistics_text_.empty())
    titleStr += " | " + statistics_text_;
  }

  window_.title = [NSString stringWithUTF8String:titleStr.c_str()];
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

void WindowMac::DrawSurface(int x, int y, Surface* surface) {
  if (!surface || !is_visible())
    return;
    
  // Get the overlay for this surface
  Overlay* overlay = nullptr;
  // Get overlays from OverlayManager parent class
  for (auto& overlay_ptr : overlays_) {
    if (overlay_ptr->view()->surface() == surface) {
      overlay = overlay_ptr;
      break;
    }
  }
  
  if (!overlay)
    return;
  
  if (platform_always_uses_cpu_renderer()) {
    // Cast to our IOSurface implementation
    auto* ioSurfaceMac = static_cast<IOSurfaceMac*>(surface);
    if (!ioSurfaceMac)
      return;
      
    // Get or create an IOSurfaceView for this overlay
    IOSurfaceView* ioSurfaceView = GetIOSurfaceViewForOverlay(overlay);
    if (!ioSurfaceView)
      return;
    
    // Update the view with the IOSurface
    [ioSurfaceView setIOSurface:ioSurfaceMac->GetIOSurface()];
    
    // Update the view's position and size
    NSRect overlayFrame = NSMakeRect(x, y, surface->width(), surface->height());
    [ioSurfaceView setFrame:overlayFrame];
    
    // If we have dirty regions, mark them for display
    ultralight::IntRect dirtyBounds = surface->dirty_bounds();
    if (!dirtyBounds.IsEmpty()) {
      NSRect dirtyRect = NSMakeRect(dirtyBounds.x(), dirtyBounds.y(), 
                                  dirtyBounds.width(), dirtyBounds.height());
      [ioSurfaceView setNeedsDisplayInRect:dirtyRect];
      
      // Clear the dirty bounds once displayed
      surface->ClearDirtyBounds();
    }
  }
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
  if (platform_always_uses_cpu_renderer()) {
    // For CPU rendering, we need to mark the regular NSView as needing display
    controller_.view.needsDisplay = YES;
  } else {
    // For GPU rendering, we use the MTLView
    controller_.metalView.needsDisplay = YES;
  }
}

void WindowMac::OnPaint(CAMetalLayer* layer) {
  MarkBeginFrame();

  if (platform_always_uses_cpu_renderer()) {
    // CPU rendering path - directly render to IOSurfaces
    MarkBeginRender();
    OverlayManager::Render();
    MarkEndRender();
    
    // Paint will call DrawSurface for each overlay
    MarkBeginDraw();
    OverlayManager::Paint();
    MarkEndDraw();
    
    MarkEndFrame();
    return;
  }
  
  // Metal GPU rendering path
  auto gpu_context = static_cast<AppMac*>(App::instance())->gpu_context();
  if (!gpu_context || !NeedsRepaint()) {
    MarkEndFrame();
    return;
  }

  MarkBeginRender();
  gpu_context->driver()->BeginSynchronize();
  OverlayManager::Render();
  gpu_context->driver()->EndSynchronize();
  MarkEndRender();

  if (gpu_context->driver()->HasCommandsPending() || OverlayManager::NeedsRepaint()) {
    MarkBeginDraw();
    gpu_context->BeginDrawing();
    gpu_context->driver()->DrawCommandList();
    OverlayManager::Paint();
    gpu_context->EndDrawing();
    [current_drawable_ present];
    MarkEndDraw();
    current_drawable_ = [layer nextDrawable];
    set_drawable_needs_clear(true);
  }

  MarkEndFrame();
}

static const char* frameMarker = "WindowMac::OnPaint";

void WindowMac::MarkBeginFrame()
{
    FrameMarkStart(frameMarker);
    frame_start_time_ = std::chrono::steady_clock::now();
}

void WindowMac::MarkEndFrame()
{
    FrameMarkEnd(frameMarker);
    using namespace std::chrono;

    auto now = std::chrono::steady_clock::now();

    // At the end of a frame, calculate the duration
    auto frame_duration = duration_cast<nanoseconds>(now - frame_start_time_);
    sum_frame_time_ += frame_duration;

    frame_count_++;
}

void WindowMac::MarkBeginRender()
{
    render_start_time_ = std::chrono::steady_clock::now();
}

void WindowMac::MarkEndRender()
{
    using namespace std::chrono;
    auto now = std::chrono::steady_clock::now();
    auto render_duration = duration_cast<nanoseconds>(now - render_start_time_);
    sum_render_time_ += render_duration;
}

void WindowMac::MarkBeginDraw()
{
    draw_start_time_ = std::chrono::steady_clock::now();
}

void WindowMac::MarkEndDraw()
{
    using namespace std::chrono;
    auto now = std::chrono::steady_clock::now();
    auto draw_duration = duration_cast<nanoseconds>(now - draw_start_time_);
    sum_draw_time_ += draw_duration;
}

void WindowMac::UpdateTitleWithStatistics()
{
    if (!frame_stats_enabled_ || frame_count_ == 0)
        return;

    // Calculate averages
    auto avg_frame_time = sum_frame_time_ / frame_count_;
    auto avg_render_time = sum_render_time_ / frame_count_;
    auto avg_draw_time = sum_draw_time_ / frame_count_;

    // Convert to milliseconds and round to the nearest tenth
    auto toMS = [](long long value) -> double {
        return std::round(value / 1000000.0 * 10.0) / 10.0;
    };

    double fps = frame_count_;
    std::stringstream title_stream;
    title_stream << (platform_always_uses_cpu_renderer() ? "CPU" : "GPU") << " Renderer | " << fps
                 << " FPS | Frame Stats (" << std::fixed << std::setprecision(1)
                 << "Render: " << toMS(avg_render_time.count()) << " ms, "
                 << "Draw: " << toMS(avg_draw_time.count()) << " ms, "
                 << "Total: " << toMS(avg_frame_time.count()) << " ms)";
    statistics_text_ = title_stream.str();

    // Force an update to re-calculate title string
    SetTitle(base_title_.c_str());

    frame_count_ = 0;
    sum_frame_time_ = std::chrono::nanoseconds(0);
    sum_render_time_ = std::chrono::nanoseconds(0);
    sum_draw_time_ = std::chrono::nanoseconds(0);
}
    
CAMetalLayer* WindowMac::layer() {
  if (platform_always_uses_cpu_renderer()) {
    // We don't use Metal layer in CPU rendering mode
    return nullptr;
  }
  return controller_.metalView.metalLayer;
}

IOSurfaceView* WindowMac::GetIOSurfaceViewForOverlay(Overlay* overlay) {
  // Check if we already have a view for this overlay
  auto it = overlay_views_.find(overlay);
  if (it != overlay_views_.end())
    return it->second;
    
  // Create a new IOSurfaceView
  Surface* surface = overlay->view()->surface();
  auto* ioSurfaceMac = static_cast<IOSurfaceMac*>(surface);
  if (!ioSurfaceMac)
    return nullptr;
    
  // Get the content view from our view controller
  NSView* contentView = controller_.view;
  
  // Create frame in parent coordinates
  NSRect frame = NSMakeRect(0, 0, surface->width(), surface->height());
  
  // Create the IOSurfaceView
  IOSurfaceView* ioSurfaceView = [[IOSurfaceView alloc] 
                               initWithFrame:frame
                               initialScale:window_scale_
                               ioSurface:ioSurfaceMac->GetIOSurface()];
  
  // Add to the content view
  [contentView addSubview:ioSurfaceView];
  
  // Store in our map for future reference
  overlay_views_[overlay] = ioSurfaceView;
  
  return ioSurfaceView;
}

void WindowMac::OnOverlayDestroyed(Overlay* overlay) {
  auto it = overlay_views_.find(overlay);
  if (it != overlay_views_.end()) {
    // Remove the view from its superview
    [it->second removeFromSuperview];
    
    // Remove from our map
    overlay_views_.erase(it);
  }
}

bool WindowMac::platform_always_uses_cpu_renderer() const {
  auto app = static_cast<AppMac*>(App::instance());
  return app->settings().force_cpu_renderer;
}

RefPtr<Window> Window::Create(Monitor* monitor, uint32_t width, uint32_t height,
  bool fullscreen, unsigned int window_flags) {
  return AdoptRef(*static_cast<Window*>(new WindowMac(monitor, width, height, fullscreen, window_flags)));
}

Window::~Window() {}

}  // namespace ultralight

