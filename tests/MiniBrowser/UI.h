#pragma once
#include <AppCore/AppCore.h>
#include "Page.h"
#include <map>
#include <memory>

using ultralight::JSObject;
using ultralight::JSArgs;
using ultralight::JSFunction;
using namespace ultralight;

class Console;

/**
* Browser UI implementation. Renders the toolbar/addressbar/tabs in top pane.
*/
class UI : public WindowListener,
           public LoadListener,
           public ViewListener {
 public:
  UI(RefPtr<Window> window);
  ~UI();
               
  // Inherited from WindowListener
  virtual bool OnKeyEvent(const ultralight::KeyEvent& evt) override;
  virtual bool OnMouseEvent(const ultralight::MouseEvent& evt) override;
  virtual void OnClose(ultralight::Window* window) override;
  virtual void OnResize(ultralight::Window* window, uint32_t width, uint32_t height) override;

  // Inherited from LoadListener
  virtual void OnDOMReady(View* caller, uint64_t frame_id,
    bool is_main_frame, const String& url) override;

  // Inherited from ViewListener
  virtual void OnChangeCursor(ultralight::View* caller, Cursor cursor) override { SetCursor(cursor); }

  // Called by UI JavaScript
  void OnBack(const JSObject& obj, const JSArgs& args);
  void OnForward(const JSObject& obj, const JSArgs& args);
  void OnRefresh(const JSObject& obj, const JSArgs& args);
  void OnStop(const JSObject& obj, const JSArgs& args);
  void OnToggleTools(const JSObject& obj, const JSArgs& args);
  void OnRequestChangeURL(const JSObject& obj, const JSArgs& args);

  RefPtr<Window> window() { return window_; }

protected:
  void CreatePage();
  void UpdateJavaScriptBindings();
  void UpdatePageNavigation(bool is_loading, bool can_go_back, bool can_go_forward);

  void SetLoading(bool is_loading);
  void SetCanGoBack(bool can_go_back);
  void SetCanGoForward(bool can_go_forward);
  void SetTitle(const String& title);
  void SetURL(const String& url);
  void SetCursor(Cursor cursor);

  Page* page() { return page_.get(); }
               
  RefPtr<View> view() { return overlay_->view(); }

  RefPtr<Window> window_;
  RefPtr<Overlay> overlay_;
  int ui_height_;
  int page_height_;
  float scale_;

  std::unique_ptr<Page> page_;
  Cursor cur_cursor_;
  bool is_resizing_inspector_;
  bool is_over_inspector_resize_drag_handle_;
  int inspector_resize_begin_height_;
  int inspector_resize_begin_mouse_y_;

  JSFunction updateBack;
  JSFunction updateForward;
  JSFunction updateLoading;
  JSFunction updateURL;
  JSFunction addTab;
  JSFunction updateTab;
  JSFunction closeTab;

  friend class Page;
};
