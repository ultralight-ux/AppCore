#include "UI.h"

static UI* g_ui = 0;

#define UI_HEIGHT 41

UI::UI(RefPtr<Window> window) : window_(window) {
  uint32_t window_width = window_->width();
  ui_height_ = (uint32_t)std::round(UI_HEIGHT * window_->scale());
  overlay_ = Overlay::Create(window_, window_width, ui_height_, 0, 0);
  g_ui = this;

  view()->set_load_listener(this);
  view()->set_view_listener(this);
  view()->LoadURL("file:///ui.html");
}

UI::~UI() {
  view()->set_load_listener(nullptr);
  view()->set_view_listener(nullptr);
  g_ui = nullptr;
}

bool UI::OnKeyEvent(const ultralight::KeyEvent& evt) {
  // Consume all F2 key events
  if (evt.virtual_key_code == KeyCodes::GK_F2) {
    if (evt.type == KeyEvent::kType_RawKeyDown) {
      App::instance()->renderer()->StartRemoteInspectorServer("127.0.0.1", 7676);
    }
    return false;
  }

  return true;
}

void UI::OnClose(ultralight::Window* window) {
  App::instance()->Quit();
}

void UI::OnResize(ultralight::Window* window, uint32_t width, uint32_t height) {
  int page_height = window->height() - ui_height_;

  if (page_height < 1)
    page_height = 1;

  overlay_->Resize(window->width(), ui_height_);

  page()->Resize(window->width(), (uint32_t)page_height);
}

void UI::OnDOMReady(View* caller, uint64_t frame_id, bool is_main_frame, const String& url) {
  // Set the context for all subsequent JS* calls
  RefPtr<JSContext> locked_context = view()->LockJSContext();
  SetJSContext(locked_context->ctx());

  JSObject global = JSGlobalObject();
  updateBack = global["updateBack"];
  updateForward = global["updateForward"];
  updateLoading = global["updateLoading"];
  updateURL = global["updateURL"];

  global["OnBack"] = BindJSCallback(&UI::OnBack);
  global["OnForward"] = BindJSCallback(&UI::OnForward);
  global["OnRefresh"] = BindJSCallback(&UI::OnRefresh);
  global["OnStop"] = BindJSCallback(&UI::OnStop);
  global["OnToggleTools"] = BindJSCallback(&UI::OnToggleTools);
  global["OnRequestChangeURL"] = BindJSCallback(&UI::OnRequestChangeURL);

  CreatePage();
}

void UI::OnBack(const JSObject& obj, const JSArgs& args) {
  page()->view()->GoBack();
}

void UI::OnForward(const JSObject& obj, const JSArgs& args) { page()->view()->GoForward(); }

void UI::OnRefresh(const JSObject& obj, const JSArgs& args) { page()->view()->Reload(); }

void UI::OnStop(const JSObject& obj, const JSArgs& args) { page()->view()->Stop(); }

void UI::OnToggleTools(const JSObject& obj, const JSArgs& args) { page()->ToggleInspector(); }

void UI::OnRequestChangeURL(const JSObject& obj, const JSArgs& args) {
  if (args.size() == 1) {
    ultralight::String url = args[0];

    page()->view()->LoadURL(url);
  }
}

void UI::CreatePage() {
  RefPtr<Window> window = window_;
  int page_height = window->height() - ui_height_;
  if (page_height < 1)
    page_height = 1;
  page_.reset(new Page(this, window->width(), (uint32_t)page_height, 0, ui_height_));
  page_->view()->LoadURL("file:///page.html");
}

void UI::UpdatePageNavigation(bool is_loading, bool can_go_back, bool can_go_forward) {
  SetLoading(is_loading);
  SetCanGoBack(can_go_back);
  SetCanGoForward(can_go_forward);
}

void UI::SetLoading(bool is_loading) {
  RefPtr<JSContext> lock(view()->LockJSContext());
  updateLoading({ is_loading });
}

void UI::SetCanGoBack(bool can_go_back) {
  RefPtr<JSContext> lock(view()->LockJSContext());
  updateBack({ can_go_back });
}

void UI::SetCanGoForward(bool can_go_forward) {
  RefPtr<JSContext> lock(view()->LockJSContext());
  updateForward({ can_go_forward });
}

void UI::SetTitle(const String& title) { window_->SetTitle(title.utf8().data()); }

void UI::SetURL(const ultralight::String& url) {
  RefPtr<JSContext> lock(view()->LockJSContext());
  updateURL({ url });
}

void UI::SetCursor(ultralight::Cursor cursor) {
  if (App::instance())
    window_->SetCursor(cursor);
}