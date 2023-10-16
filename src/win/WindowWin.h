#pragma once
#include <AppCore/Window.h>
#include <Ultralight/Listener.h>
#include "RefCountedImpl.h"
#include "OverlayManager.h"
#include <Windows.h>
#include <memory>
#if defined(DRIVER_D3D11)
#include "d3d11/SwapChainD3D11.h"
#endif

namespace ultralight {

struct WindowData {
  class WindowWin* window;
  MouseEvent::Button cur_btn;
  bool is_resizing_modal;
  bool is_mouse_in_client;
  bool is_tracking_mouse;
  TRACKMOUSEEVENT track_mouse_event_data;
};

class WindowWin : public Window, public RefCountedImpl<WindowWin>, public OverlayManager {
 public:
  virtual void set_listener(WindowListener* listener) override { listener_ = listener; }

  virtual WindowListener* listener() override { return listener_; }

  virtual uint32_t screen_width() const override;

  virtual uint32_t width() const override;

  virtual uint32_t screen_height() const override;

  virtual uint32_t height() const override;

  virtual bool is_fullscreen() const override { return is_fullscreen_; }

  virtual bool is_accelerated() const override { return is_accelerated_; }

  virtual uint32_t render_buffer_id() const override {
    return swap_chain_ ? swap_chain_->render_buffer_id() : 0;
  }

  virtual double scale() const override;

  virtual void MoveTo(int x, int y) override;

  virtual void MoveToCenter() override;

  virtual int x() const override;

  virtual int y() const override;

  virtual void SetTitle(const char* title) override;

  virtual void SetCursor(ultralight::Cursor cursor) override;

  virtual void Show() override;

  virtual void Hide() override;

  virtual bool is_visible() const override;

  virtual void Close() override;

  virtual int ScreenToPixels(int val) const override { return (int)round(val * scale()); }

  virtual int PixelsToScreen(int val) const override { return (int)round(val / scale()); }

  virtual void DrawSurface(int x, int y, Surface* surface) override;

  virtual void* native_handle() const override;

  virtual OverlayManager* overlay_manager() const { return const_cast<WindowWin*>(this); }

  // Inherited from OverlayManager
  virtual void Paint() override;

  virtual void FireKeyEvent(const ultralight::KeyEvent& evt) override;

  virtual void FireMouseEvent(const ultralight::MouseEvent& evt) override;

  virtual void FireScrollEvent(const ultralight::ScrollEvent& evt) override;

  REF_COUNTED_IMPL(WindowWin);

  HWND hwnd() { return hwnd_; }

  void set_app_listener(WindowListener* listener) { app_listener_ = listener; }

  // These are called by WndProc then forwarded to listener(s)
  void OnClose();
  void OnResize(uint32_t width, uint32_t height);
  void OnChangeDPI(double scale, const RECT* suggested_rect);

  void InvalidateWindow() { window_needs_repaint_ = true; }

  // Should be called at end of app lifetime to cleanup WINAPI window class
  static void CleanupWindowClass();

 protected:
  WindowWin(Monitor* monitor, uint32_t width, uint32_t height, bool fullscreen,
            unsigned int window_flags);

  virtual ~WindowWin();

  virtual bool platform_always_uses_cpu_renderer() const override { return false; }

  friend class Window;

  DISALLOW_COPY_AND_ASSIGN(WindowWin);

  WindowListener* listener_ = nullptr;
  WindowListener* app_listener_ = nullptr;
  bool is_first_paint_ = true;
  bool window_needs_repaint_ = false;
  Monitor* monitor_;
  double scale_;
  bool is_fullscreen_;
  bool is_accelerated_;
  HWND hwnd_;
  HCURSOR cursor_hand_;
  HCURSOR cursor_arrow_;
  HCURSOR cursor_ibeam_;
  HCURSOR cursor_size_all_;
  HCURSOR cursor_size_north_east_;
  HCURSOR cursor_size_north_south_;
  HCURSOR cursor_size_north_west_;
  HCURSOR cursor_size_west_east_;
  Cursor cur_cursor_;
  WindowData window_data_;
  DWORD style_;

#if defined(DRIVER_D3D11)
  std::unique_ptr<SwapChainD3D11> swap_chain_;
#endif
};

} // namespace ultralight
