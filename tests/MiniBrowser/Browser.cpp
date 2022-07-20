#include "Browser.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>
#include <Ultralight/Renderer.h>

Browser::Browser()  {
  Settings settings;
#ifdef _WIN32
  settings.force_cpu_renderer = true;
#endif
  Config config;
  config.scroll_timer_delay = 1.0 / 90.0;
  app_ = App::Create(settings, config);
    
  window_ = Window::Create(app_->main_monitor(), 1024, 768, false, 
    kWindowFlags_Resizable | kWindowFlags_Titled | kWindowFlags_Maximizable);
  window_->SetTitle("Ultralight MiniBrowser");

  // Create the UI
  ui_.reset(new UI(window_));
  window_->set_listener(ui_.get());
}

Browser::~Browser() {
  window_->set_listener(nullptr);

  ui_.reset();

  window_ = nullptr;
  app_ = nullptr;
}

void Browser::Run() {
  app_->Run();
}
