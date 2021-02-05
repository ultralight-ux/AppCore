#pragma once
#include <AppCore/Defines.h>
#include <Ultralight/KeyEvent.h>
#include <Ultralight/MouseEvent.h>
#include <Ultralight/ScrollEvent.h>
#include <vector>

namespace ultralight {

class Overlay;

class OverlayManager {
public:
  OverlayManager();
  virtual ~OverlayManager();

  virtual void Add(Overlay* overlay);

  virtual void Remove(Overlay* overlay);

  // Render all active Views.
  virtual void Render();

  // Repaint overlays
  virtual void Paint();

  virtual void SetWindowFocused(bool focused);

  virtual void SetWindowScale(double scale);

  virtual void FireKeyEvent(const ultralight::KeyEvent& evt);

  virtual void FireMouseEvent(const ultralight::MouseEvent& evt);

  virtual void FireScrollEvent(const ultralight::ScrollEvent& evt);

  virtual void FocusOverlay(Overlay* overlay);

  virtual void UnfocusAll();

  virtual bool IsOverlayFocused(Overlay* overlay) const;
    
  virtual bool NeedsRepaint();

protected:
  Overlay* HitTest(int x, int y);

  std::vector<Overlay*> overlays_;
  Overlay* focused_overlay_ = nullptr;
  Overlay* hovered_overlay_ = nullptr;
  bool is_dragging_ = false;
  bool window_focused_ = false;
  double window_scale_ = 1.0;
};

}  // namespace ultralight
