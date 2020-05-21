#pragma once
#include <Ultralight/platform/GPUDriver.h>
#include <Ultralight/platform/Config.h>
#include "GPUDriverImpl.h"
#include <memory>

typedef struct GLFWwindow GLFWwindow;

#define ENABLE_OFFSCREEN_GL 0

namespace ultralight {

class GPUContextGL {
protected:
  std::unique_ptr<ultralight::GPUDriverImpl> driver_;
  GLFWwindow* window_;
  float scale_;
  bool msaa_enabled_;
public:
  GPUContextGL(GLFWwindow* window, float scale, bool enable_vsync, bool enable_msaa);

  virtual ~GPUContextGL() {}

  virtual ultralight::GPUDriverImpl* driver() const { return driver_.get(); }

  virtual ultralight::FaceWinding face_winding() const { return ultralight::kFaceWinding_CounterClockwise; }

  virtual void BeginDrawing() {}

  virtual void EndDrawing() {}

  // Should only be called during drawing phase
  virtual void PresentFrame();

  virtual void Resize(int width, int height);

  virtual bool msaa_enabled() const { return msaa_enabled_; }

  virtual float scale() const { return scale_; }
};

}  // namespace ultralight