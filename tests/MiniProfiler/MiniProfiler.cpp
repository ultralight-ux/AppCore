#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include <AppCore/JSHelpers.h>
#include <Ultralight/Defines.h>
#include <Ultralight/platform/Logger.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/private/util/MemoryTag.h>
#include <Ultralight/private/JavaScriptVM.h>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <queue>

#define ENABLE_PERIODIC_MEMORY_SHRINK 0

using namespace ultralight;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define STATS_WIDTH 450

class MyApp : public WindowListener,
              public ViewListener,
              public LoadListener,
              public AppListener,
              public Logger {
  RefPtr<App> app_;
  RefPtr<Window> window_;
  RefPtr<Overlay> content_;
  RefPtr<Overlay> stats_;
  JSFunction updateStats;
  JSFunction addMessage;
  std::chrono::steady_clock::time_point lastUpdate_;
  std::chrono::steady_clock::time_point lastShrink_;
  std::queue<String> logQueue_;

 public:
  MyApp() {
    lastUpdate_ = std::chrono::steady_clock::now();
    lastShrink_ = std::chrono::steady_clock::now();

    Platform::instance().set_logger(this);

    Settings s;
    s.force_cpu_renderer = true;
    Config c;
    app_ = App::Create(s, c);
    app_->set_listener(this);

    CreateWindow();
  }

  virtual ~MyApp() { Platform::instance().set_logger(nullptr); }

  void CreateWindow() {
    window_ = Window::Create(app_->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT, false,
                             kWindowFlags_Titled | kWindowFlags_Resizable);
    window_->set_listener(this);
    window_->SetTitle("Ultralight - MiniProfiler");

    ViewConfig viewConfig;
    viewConfig.initial_device_scale = window_->scale();
    auto session = app_->renderer()->CreateSession(false, "");
    auto view = app_->renderer()->CreateView(100, 100, viewConfig, session);
    content_ = Overlay::Create(window_, view, 0, 0);
    content_->view()->set_view_listener(this);
    content_->view()->set_load_listener(this);
    content_->view()->LoadURL("file:///home.html");

    stats_ = Overlay::Create(window_, 100, 100, 0, 0);
    stats_->view()->set_view_listener(this);
    stats_->view()->set_load_listener(this);
    stats_->view()->LoadURL("file:///stats.html");

    OnResize(window_.get(), window_->width(), window_->height());
  }

  virtual void OnUpdate() override {
    auto sinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - lastUpdate_);

    if (sinceLastUpdate > std::chrono::milliseconds(500)) {
      UpdateStats();
      lastUpdate_ = std::chrono::steady_clock::now();
    }

    auto sinceLastShrink = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - lastShrink_);

    if (sinceLastShrink > std::chrono::milliseconds(3000)) {
      ShrinkMemory();
      lastShrink_ = std::chrono::steady_clock::now();
    }
  }

  ///
  /// Inherited from WindowListener, called when the window is closed.
  ///
  virtual void OnClose(ultralight::Window* window) override {
    window_ = nullptr;
    app_->Quit();
  }

  ///
  /// Inherited from WindowListener, called when the Window is resized.
  ///
  virtual void OnResize(ultralight::Window* window, uint32_t width, uint32_t height) override {
    uint32_t right_pane_width_px = window_->ScreenToPixels(STATS_WIDTH);
    stats_->Resize(right_pane_width_px, height);

    // Calculate the width of our left pane (window width - right width)
    int left_pane_width = (int)width - right_pane_width_px;

    // Clamp our right pane's width to a minimum of 1
    left_pane_width = left_pane_width > 1 ? left_pane_width : 1;

    content_->Resize((uint32_t)left_pane_width, height);

    content_->MoveTo(0, 0);
    stats_->MoveTo(left_pane_width, 0);
  }

  ///
  /// Inherited from ViewListener
  ///
  virtual void OnChangeCursor(ultralight::View* caller, Cursor cursor) override {
    ///
    /// Update the active cursor for the window.
    ///
    if (window_)
      window_->SetCursor(cursor);
  }

  virtual void OnBeginLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
                              const String& url) {
    if (caller == content_->view().get() && is_main_frame) {
      AddMessage("Begin Loading URL: " + url);
    }
  }

  virtual void OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
                               const String& url) {
    if (caller == content_->view().get() && is_main_frame) {
      AddMessage("Finish Loading URL: " + url);
    }
  }

  virtual void OnFailLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
                             const String& url, const String& description,
                             const String& error_domain, int error_code) {
    if (caller == content_->view().get() && is_main_frame) {
      AddMessage("Failed Loading URL: " + url + ", error: " + description);
    }
  }

  virtual void OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
                          const String& url) override {
    if (caller == stats_->view().get()) {
      RefPtr<JSContext> context = caller->LockJSContext();
      SetJSContext(context->ctx());

      JSObject global = JSGlobalObject();
      updateStats = global["updateStats"];
      addMessage = global["addMessage"];
      global["ResetView"] = BindJSCallback(&MyApp::ResetView);
    }
  }

  virtual void OnAddConsoleMessage(ultralight::View* caller, MessageSource source,
                                   MessageLevel level, const String& message, uint32_t line_number,
                                   uint32_t column_number, const String& source_id) override {
    if (caller == content_->view().get())
      AddMessage(message);
  }

  virtual void LogMessage(LogLevel log_level, const String& message) override {
    AddMessage(message);
  }

  void ResetView(const JSObject& thisObject, const JSArgs& args) {
    auto url = content_->view()->url();
    content_->view()->set_view_listener(nullptr);
    content_->view()->set_load_listener(nullptr);
    content_ = nullptr;

    ViewConfig viewConfig;
    viewConfig.initial_device_scale = window_->scale();
    auto session = app_->renderer()->CreateSession(false, "");
    auto view = app_->renderer()->CreateView(100, 100, viewConfig, session);
    content_ = Overlay::Create(window_, view, 0, 0);
    content_->view()->set_view_listener(this);
    content_->view()->set_load_listener(this);
    content_->view()->LoadURL("file:///home.html");

    OnResize(window_.get(), window_->width(), window_->height());
  }

  void UpdateStats() {
    if (!updateStats)
      return;

    JSArray data;

    size_t tag_count = static_cast<size_t>(MemoryTag::Count);
    int64_t totalBytes = 0;
    for (size_t i = 0; i < tag_count; i++) {
      MemoryTag tag = static_cast<MemoryTag>(i);
      int64_t bytes = GetMemoryStats(tag);
      totalBytes += bytes;

      // Only show tags that have > 100 KB allocated.
      if (bytes < 100 * 1024)
        continue;

      data.push(JSValue(JSArray({ MemoryTagToString(tag), bytes })));
    }

    data.push(JSValue(JSArray({ "Total Tracked: ", totalBytes })));

    updateStats({ JSValue(data) });

    HandlePendingMessages();
  }

  void AddMessage(const ultralight::String& message) {
    if (!addMessage) {
      logQueue_.push(message);
      return;
    }

    HandlePendingMessages();

    addMessage({ JSValue(message) });
  }

  void HandlePendingMessages() {
    if (addMessage) {
      while (logQueue_.size()) {
        addMessage({ JSValue(logQueue_.front()) });
        logQueue_.pop();
      }
    }
  }

  void ShrinkMemory() {
#if ENABLE_PERIODIC_MEMORY_SHRINK
    auto vm = content_->view()->JavaScriptVM();
    JavaScriptVM::ShrinkFootprintWhenIdle(vm);

    app_->renderer()->PurgeMemory();
#endif
  }

  void Run() {
    app_->Run();
  }
};

#include <Windows.h>

int main() {

  MyApp app;
  app.Run();

  return 0;
}