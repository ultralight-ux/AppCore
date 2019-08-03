#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>

using namespace ultralight;

int main() {
  ///
  /// Create our main App instance.
  ///
  Config config;
  config.force_repaint = true;
  auto app = App::Create(Settings(), config);
  
  ///
  /// Create our Window using default window flags.
  ///
  auto window = Window::Create(app->main_monitor(), 900, 600, false, kWindowFlags_Titled);

  ///
  /// Set our window title.
  ///
  window->SetTitle("MiniBrowser");

  ///
  /// Bind our App's main window.
  ///
  /// @note This MUST be done before creating any overlays or calling App::Run
  ///
  app->set_window(window);

  ///
  /// Create our Overlay, use the same dimensions as our Window.
  ///
  auto overlay = Overlay::Create(window, window->width(), window->height(), 0, 0);

  ///
  /// Load a string of HTML into our overlay's View
  ///
    overlay->view()->LoadHTML(R"(<!DOCTYPE html>
                              <html>
                              <style>
                              input[type=text], select {
                              width: 100%;
                              padding: 12px 20px;
                              margin: 8px 0;
                              display: inline-block;
                              border: 1px solid #ccc;
                                  border-radius: 4px;
                                  box-sizing: border-box;
                              }
                              
                              input[type=submit] {
                              width: 100%;
                                  background-color: #4CAF50;
                              color: white;
                              padding: 14px 20px;
                              margin: 8px 0;
                              border: none;
                                  border-radius: 4px;
                              cursor: pointer;
                              }
                              
                              input[type=submit]:hover {
                                  background-color: #45a049;
                              }
                              
                              div {
                                  border-radius: 5px;
                                  background-color: #f2f2f2;
                                  background: linear-gradient(#FED, #FFE);
                              padding: 20px;
                              }
                              </style>
                              <body>
                              
                              <h3>Using CSS to style an HTML Form</h3>
                              
                              <div>
                              <form action="/action_page.php">
                              <label for="fname">First Name</label>
                              <input type="text" id="fname" name="firstname" placeholder="Your name..">
                              
                              <label for="lname">Last Name</label>
                              <input type="text" id="lname" name="lastname" placeholder="Your last name..">
                              
                              <label for="country">Country</label>
                              <select id="country" name="country">
                              <option value="australia">Australia</option>
                              <option value="canada">Canada</option>
                              <option value="usa">USA</option>
                              </select>
                              
                              <input type="submit" value="Submit">
                              </form>
                              </div>
                              
                              </body>
                              </html>
)");
    //overlay->view()->LoadURL("https://davidwalsh.name/demo/css-cube.php");
    //overlay->view()->LoadURL("https://paulrhayes.com/experiments/cube-3d/touch.html");
  ///
  /// Run our main loop.
  ///
  app->Run();

  return 0;
}
