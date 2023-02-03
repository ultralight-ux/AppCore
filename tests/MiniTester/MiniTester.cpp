#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include <Ultralight/KeyCodes.h>
#include <iostream>
#include <vector>

using namespace ultralight;

struct AppWindow {
  RefPtr<Window> window_;
  RefPtr<Overlay> overlay_;
};

class MyApp : public WindowListener, public ViewListener, public LoadListener {
  RefPtr<App> app_;
  std::vector<AppWindow> active_windows_;

 public:
  MyApp() {
    Settings s;
#ifdef _WIN32
    s.force_cpu_renderer = true;
#endif
    Config c;
    app_ = App::Create(s, c);

    CreateWindow();
  }

  virtual ~MyApp() { }

  void CreateWindow() {
    auto window = Window::Create(app_->main_monitor(), 800, 600, false,
                                 kWindowFlags_Titled | kWindowFlags_Resizable);
    window->set_listener(this);
    window->SetTitle("MiniBrowser");

    auto overlay = Overlay::Create(window, window->width(), window->height(), 0, 0);
    overlay->view()->set_view_listener(this);
    overlay->view()->set_load_listener(this);
    overlay->view()->LoadURL("file:///home.html");

    active_windows_.push_back({ window, overlay });
  }

  ///
  /// Inherited from WindowListener, called when the window is closed.
  ///
  virtual void OnClose(ultralight::Window* window) override {
    for (auto i = active_windows_.begin(); i != active_windows_.end();) {
      if (i->window_.get() == window) {
        i = active_windows_.erase(i);
      } else {
        i++;
      }
    }

    if (active_windows_.size() == 0)
      app_->Quit();
  }

  ///
  /// Inherited from WindowListener, called when the Window is resized.
  ///
  virtual void OnResize(ultralight::Window* window, uint32_t width, uint32_t height) override {
    ///
    /// Find the overlay that matches the window emitting the resize event.
    ///
    Overlay* overlay = nullptr;
    for (auto i : active_windows_) {
      if (i.window_.get() == window) {
        overlay = i.overlay_.get();
        break;
      }
    }

    ///
    /// Resize the overlay to match the new window dimensions.
    ///
    if (overlay)
      overlay->Resize(width, height);
  }

  ///
  /// Inherited from ViewListener
  ///
  virtual void OnChangeCursor(ultralight::View* caller, Cursor cursor) override {
    ///
    /// Find the window that matches the view emitting the cursor event.
    ///
    Window* window = nullptr;
    for (auto i : active_windows_) {
      if (i.overlay_->view().get() == caller) {
        window = i.window_.get();
        break;
      }
    }

    ///
    /// Update the active cursor for the window.
    ///
    if (window)
      window->SetCursor(cursor);
  }

  virtual void OnBeginLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
                              const String& url) override {
    std::cout << "[OnBeginLoading]\n\t"
              << "\n\tframe_id:\t" << frame_id
              << "\n\tis_main_frame:\t" << is_main_frame
              << "\n\turl:\t" << url.utf8().data()
              << std::endl;
  }

  virtual void OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
                               const String& url) {
      std::cout << "[OnFinishLoading]\n\t"
              << "\n\tframe_id:\t" << frame_id
              << "\n\tis_main_frame:\t" << is_main_frame
              << "\n\turl:\t" << url.utf8().data()
              << std::endl;
  }

  virtual void OnFailLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
                             const String& url, const String& description,
                             const String& error_domain, int error_code) {
        std::cout << "[OnFailLoading]\n\t"
              << "\n\tframe_id:\t" << frame_id
              << "\n\tis_main_frame:\t" << is_main_frame
              << "\n\turl:\t" << url.utf8().data()
              << "\n\tdescription:\t" << description.utf8().data()
              << "\n\terror_domain:\t" << error_domain.utf8().data()
              << "\n\terror_code:\t" << error_code
              << std::endl;
  }

  virtual void OnWindowObjectReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
                                   const String& url) {
      std::cout << "[OnWindowObjectReady]\n\t"
              << "\n\tframe_id:\t" << frame_id
              << "\n\tis_main_frame:\t" << is_main_frame
              << "\n\turl:\t" << url.utf8().data()
              << std::endl;
  }

  virtual void OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
                          const String& url) {
     std::cout << "[OnDOMReady]\n\t"
             << "\n\tframe_id:\t" << frame_id
             << "\n\tis_main_frame:\t" << is_main_frame
             << "\n\turl:\t" << url.utf8().data()
             << std::endl;
  }

  void Run() { app_->Run(); }
};

int main() {
  MyApp app;
  app.Run();

  return 0;
}