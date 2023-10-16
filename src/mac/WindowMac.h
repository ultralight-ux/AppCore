#pragma once
#include <AppCore/Window.h>
#include <Ultralight/Listener.h>
#include "RefCountedImpl.h"
#include "OverlayManager.h"
#import "Cocoa/Cocoa.h"
#import "ViewController.h"
#import "WindowDelegate.h"
#import <MetalKit/MetalKit.h>
#import <QuartzCore/QuartzCore.h>

namespace ultralight {

class WindowMac : public Window,
                  public RefCountedImpl<WindowMac>,
                  public OverlayManager {
public:
  virtual void set_listener(WindowListener* listener) override { listener_ = listener; }

  virtual WindowListener* listener() override { return listener_; }

  virtual uint32_t screen_width() const override;

  virtual uint32_t width() const override;

  virtual uint32_t screen_height() const override;

  virtual uint32_t height() const override;
  
  virtual void MoveTo(int x, int y) override;
  
  virtual void MoveToCenter() override;

  virtual int x() const override;

  virtual int y() const override;

  virtual bool is_fullscreen() const override { return is_fullscreen_; }
                    
  virtual bool is_accelerated() const override { return is_accelerated_; }
                
  virtual uint32_t render_buffer_id() const override { return render_buffer_id_; }

  virtual double scale() const override;

  virtual void SetTitle(const char* title) override;

  virtual void SetCursor(ultralight::Cursor cursor) override;
  
  virtual void Show() override;

  virtual void Hide() override;

  virtual bool is_visible() const override;

  virtual void Close() override;

  virtual int ScreenToPixels(int val) const override {
    return (int)round(val * scale());
  }

  virtual int PixelsToScreen(int val) const override {
    return (int)round(val / scale());
  }

  virtual void* native_handle() const override;

  virtual void DrawSurface(int x, int y, Surface* surface) override;

  virtual OverlayManager* overlay_manager() const override { return const_cast<WindowMac*>(this); }

  virtual void FireKeyEvent(const ultralight::KeyEvent& evt) override;

  virtual void FireMouseEvent(const ultralight::MouseEvent& evt) override;

  virtual void FireScrollEvent(const ultralight::ScrollEvent& evt) override;

  REF_COUNTED_IMPL(WindowMac);

  void set_app_listener(WindowListener* listener) { app_listener_ = listener; }
                      
  void OnClose();
  
  void OnResize(uint32_t width, uint32_t height);
                      
  void SetNeedsDisplay();
  
  void OnPaint(CAMetalLayer* layer);
                      
  CAMetalLayer* layer();
  
  id <CAMetalDrawable> current_drawable() { return current_drawable_; }

protected:
  WindowMac(Monitor* monitor, uint32_t width, uint32_t height,
    bool fullscreen, unsigned int window_flags);

  virtual ~WindowMac();

  virtual bool platform_always_uses_cpu_renderer() const override { return true; }

  friend class Window;

  DISALLOW_COPY_AND_ASSIGN(WindowMac);

  NSWindow* window_;
  ViewController* controller_;
  WindowDelegate* delegate_;
  WindowListener* listener_ = nullptr;
  WindowListener* app_listener_ = nullptr;
  Monitor* monitor_;
  bool is_fullscreen_;
  bool is_accelerated_;
  uint32_t render_buffer_id_;
  id <CAMetalDrawable> current_drawable_;
};

}  // namespace ultralight

