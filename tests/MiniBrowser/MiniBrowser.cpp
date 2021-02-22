#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include <vector>

using namespace ultralight;

struct AppWindow {
  RefPtr<Window> window_;
  RefPtr<Overlay> overlay_;
};

class MyApp : public WindowListener, public ViewListener {
  RefPtr<App> app_;
  std::vector<AppWindow> active_windows_;

 public:
  MyApp() {
    Settings s;
    s.force_cpu_renderer = false;
    app_ = App::Create(s);

    CreateWindow();
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
      if (i.overlay_->view().ptr() == caller) {
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

  void Run() { app_->Run(); }
};

int main() {
  MyApp app;
  app.Run();

  return 0;
}
