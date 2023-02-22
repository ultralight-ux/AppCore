#include "GPUContextGL.h"
#include "GPUDriverGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ultralight {

GPUContextGL::GPUContextGL(bool enable_vsync, bool enable_msaa) : 
  msaa_enabled_(enable_msaa) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  if (enable_msaa) {
    // Request 4x MSAA for our window
    glfwWindowHint(GLFW_SAMPLES, 4);
  }

  // Make the window offscreen
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  GLFWwindow* win = glfwCreateWindow(10, 10, "", NULL, NULL);
  window_ = win;
  if (!window_)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window_);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glfwSwapInterval(enable_vsync ? 1 : 0);

  int samples = 4;
  glGetIntegerv(GL_SAMPLES, &samples);
  if (!samples) {
    msaa_enabled_ = false;
  }

  if (msaa_enabled_) {
    glEnable(GL_MULTISAMPLE);
  }

  driver_.reset(new ultralight::GPUDriverGL(this));
}

}  // namespace ultralight
