#include "WindowMac.h"
#include <AppCore/Monitor.h>
#include <AppKit/NSCursor.h>
#include "AppMac.h"
#import "Cocoa/Cocoa.h"
#import "ViewController.h"

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
  [window_ center];
  [window_ setBackgroundColor:[NSColor whiteColor]];
  [window_ makeKeyAndOrderFront:NSApp];
  [window_ setOrderedIndex:0];
  [NSApp activateIgnoringOtherApps:YES];

  controller_ = [[ViewController alloc] init];
  [controller_ initWithWindow:this frame:frame];
  [window_ setContentViewController:controller_];

  delegate_ = [[WindowDelegate alloc] init];
  [delegate_ initWithWindow:this];
  [window_ setDelegate:delegate_];

  // Move app to foreground
  ProcessSerialNumber psn = {0, kCurrentProcess};
  TransformProcessType(&psn, kProcessTransformToForegroundApplication);
}

WindowMac::~WindowMac() {
}

uint32_t WindowMac::width() const {
  return (uint32_t)PixelsToDevice((int)[controller_ metalView].drawableSize.width);
}

uint32_t WindowMac::height() const {
  return (uint32_t)PixelsToDevice((int)[controller_ metalView].drawableSize.height);
}

double WindowMac::scale() const {
  return monitor_->scale();
}

void WindowMac::SetTitle(const char* title) {
  window_.title = [NSString stringWithUTF8String:title];
}

void WindowMac::SetCursor(ultralight::Cursor cursor) {
    switch (cursor) {
        case ultralight::kCursor_Pointer:
            [[NSCursor arrowCursor] set];
            break;

        case ultralight::kCursor_Cross:
            [[NSCursor crosshairCursor] set];
            break;

        case ultralight::kCursor_Hand:
            [[NSCursor pointingHandCursor] set];
            break;

        case ultralight::kCursor_IBeam:
            [[NSCursor IBeamCursor] set];
            break;

        case ultralight::kCursor_Wait:
            break;


        case ultralight::kCursor_Help:
            break;

        case ultralight::kCursor_EastResize:
            [[NSCursor resizeRightCursor] set];
            break;

        case ultralight::kCursor_NorthResize:
            [[NSCursor resizeUpCursor] set];
            break;

        case ultralight::kCursor_NorthEastResize:
            break;

        case ultralight::kCursor_NorthWestResize:
            break;

        case ultralight::kCursor_SouthResize:
            [[NSCursor resizeDownCursor] set];
            break;

        case ultralight::kCursor_SouthEastResize:
            break;

        case ultralight::kCursor_SouthWestResize:
            break;


        case ultralight::kCursor_WestResize:
            [[NSCursor resizeLeftCursor] set];
            break;

        case ultralight::kCursor_NorthSouthResize:
            [[NSCursor resizeUpDownCursor] set];
            break;

        case ultralight::kCursor_EastWestResize:
            [[NSCursor resizeLeftRightCursor] set];
            break;

        case ultralight::kCursor_NorthEastSouthWestResize:
        case ultralight::kCursor_NorthWestSouthEastResize:
        case ultralight::kCursor_ColumnResize:
        case ultralight::kCursor_RowResize:
        case ultralight::kCursor_MiddlePanning:
        case ultralight::kCursor_EastPanning:
        case ultralight::kCursor_NorthPanning:
        case ultralight::kCursor_NorthEastPanning:
        case ultralight::kCursor_NorthWestPanning:
        case ultralight::kCursor_SouthPanning:
        case ultralight::kCursor_SouthEastPanning:
        case ultralight::kCursor_SouthWestPanning:
        case ultralight::kCursor_Move:
        case ultralight::kCursor_WestPanning:
            break;

        case ultralight::kCursor_VerticalText:
            [[NSCursor IBeamCursorForVerticalLayout] set];
            break;

        case ultralight::kCursor_Cell:
            break;

        case ultralight::kCursor_ContextMenu:
            [[NSCursor contextualMenuCursor] set];
            break;

        case ultralight::kCursor_Alias:
        case ultralight::kCursor_Progress:
        case ultralight::kCursor_NoDrop:
        case ultralight::kCursor_Copy:
        case ultralight::kCursor_None:
            break;

        case ultralight::kCursor_NotAllowed:
            [[NSCursor operationNotAllowedCursor] set];
            break;

        case ultralight::kCursor_ZoomIn:
        case ultralight::kCursor_ZoomOut:
        case ultralight::kCursor_Grab:
        case ultralight::kCursor_Grabbing:
            break;

        default:
            break;
    }
}

void WindowMac::Close() {
  [window_ close];
}

void WindowMac::OnClose() {
  if (listener_)
    listener_->OnClose();
  if (app_listener_)
    app_listener_->OnClose();
}

void WindowMac::OnResize(uint32_t width, uint32_t height) {
  [controller_ metalView].drawableSize = CGSizeMake(DeviceToPixels((int)width), DeviceToPixels((int)height));

  if (listener_)
    listener_->OnResize(width, height);
  if (app_listener_)
    app_listener_->OnResize(width, height);
}

MTKView* WindowMac::view() {
    return [controller_ metalView];
}

Ref<Window> Window::Create(Monitor* monitor, uint32_t width, uint32_t height,
  bool fullscreen, unsigned int window_flags) {
  return AdoptRef(*new WindowMac(monitor, width, height, fullscreen, window_flags));
}

Window::~Window() {}

}  // namespace ultralight

