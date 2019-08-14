#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>

using namespace ultralight;

int main() {
  auto app = App::Create();
  auto window = Window::Create(app->main_monitor(), 900, 600, false, kWindowFlags_Titled);
  window->SetTitle("MiniBrowser");
  app->set_window(window);
  auto overlay = Overlay::Create(window, window->width(), window->height(), 0, 0);
  overlay->view()->LoadURL("https://google.com");

  app->Run();

  return 0;
}
