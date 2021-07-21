#pragma once
#include <Ultralight/platform/GPUDriver.h>
#include <Ultralight/platform/Config.h>
#include "GPUDriverImpl.h"
#include <memory>
#include <vulkan/vulkan.h>

typedef struct GLFWwindow GLFWwindow;

namespace ultralight {

class GPUContextVK {
protected:
  std::unique_ptr<ultralight::GPUDriverImpl> driver_;
  GLFWwindow* window_;
  GLFWwindow* active_window_ = nullptr;
  bool msaa_enabled_;
public:
  GPUContextVK(bool enable_vsync, bool enable_msaa);

  virtual ~GPUContextVK();

  virtual ultralight::GPUDriverImpl* driver() const { return driver_.get(); }

  virtual ultralight::FaceWinding face_winding() const { return ultralight::kFaceWinding_CounterClockwise; }

  virtual void BeginDrawing() {}

  virtual void EndDrawing() {}

  virtual bool msaa_enabled() const { return msaa_enabled_; }

  // An offscreen window dedicated to maintaining the OpenGL context.
  // All other windows created during lifetime of the app share this context.
  virtual GLFWwindow* window() { return window_; }

  // FBOs are not shared across contexts in OpenGL 3.2 (AFAIK), we luckily
  // don't need to share them across multiple windows anyways so we temporarily
  // set the active GL context to the "active window" when creating FBOs.
  virtual void set_active_window(GLFWwindow* win) { active_window_ = win; }

  virtual GLFWwindow* active_window() { return active_window_; }
  
  VkInstance instance;
  VkDevice device;
};

}  // namespace ultralight