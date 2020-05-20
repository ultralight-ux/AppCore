#pragma once
#import <MetalKit/MetalKit.h>
#import <Ultralight/platform/GPUDriver.h>
#import <Ultralight/platform/Config.h>
#include "GPUDriverImpl.h"
#include <memory>
#include <map>

namespace ultralight {
    
class GPUDriverMetal;
    
struct RenderState {
  RenderState();
  
  size_t Hash();
  
  ShaderType shader_type;
  bool blend_enabled;
  MTLPixelFormat pixel_format;
  NSUInteger sample_count;
};
    
class GPUContextMetal {
 public:
  GPUContextMetal(id<MTLDevice> device, int screen_width, int screen_height, double screen_scale, bool fullscreen, bool enable_vsync, bool enable_msaa);
  
  virtual ~GPUContextMetal();
  
  void set_current_drawable(id<CAMetalDrawable> drawable);
  
  id<CAMetalDrawable> current_drawable() { return current_drawable_; }

  // Inherited from GPUContext
  virtual ultralight::GPUDriverImpl* driver() const { return (GPUDriverImpl*)driver_.get(); }
  
  virtual ultralight::FaceWinding face_winding() const {
    return ultralight::kFaceWinding_CounterClockwise;
  }
  
  virtual void BeginDrawing();
  
  virtual void EndDrawing();
  
  virtual void PresentFrame();
  
  virtual void Resize(int width, int height);
  
  virtual bool msaa_enabled() const { return msaa_enabled_; }
  
  virtual void set_shader_type(ShaderType type) { render_state_.shader_type = type; }
  virtual ShaderType shader_type() const { return render_state_.shader_type; }
  
  virtual void set_blend_enabled(bool val) { render_state_.blend_enabled = val; }
  virtual bool blend_enabled() const { return render_state_.blend_enabled; }
  
  virtual void set_pixel_format(MTLPixelFormat format) { render_state_.pixel_format = format; }
  virtual MTLPixelFormat pixel_format() { return render_state_.pixel_format; }
  
  virtual void set_sample_count(NSUInteger count) { render_state_.sample_count = count; }
  virtual NSUInteger sample_count() { return render_state_.sample_count; }
  
  // Inherited from PlatformGPUContext
  virtual id<MTLDevice> device() { return device_; }
  virtual id<MTLRenderPipelineState> render_pipeline_state();

  virtual id<MTLCommandBuffer> command_buffer();
  virtual void CommitCommandBuffer();
  virtual void set_scale(double scale) { scale_ = scale; }
  virtual double scale() const { return scale_; }
  
  virtual void set_screen_size(uint32_t width, uint32_t height) {
    width_ = width;
    height_ = height;
  }
  
  virtual void screen_size(uint32_t& width, uint32_t& height) {
    width = width_;
    height = height_;
  }
  
  static const NSUInteger FrameCount = 3;
  
protected:
  dispatch_semaphore_t fence_;
  NSUInteger frame_index_ = 0;
  
  struct Frame {
  } frames_[FrameCount];
  
  id<MTLDevice> device_;
  id<CAMetalDrawable> current_drawable_;
  id<MTLLibrary> library_;
  RenderState render_state_;
  std::map<size_t, id<MTLRenderPipelineState>> render_pipeline_states_;
  id<MTLCommandQueue> command_queue_;
  id<MTLCommandBuffer> command_buffer_;
  CAMetalLayer* layer_;
  double scale_;
  uint32_t width_;
  uint32_t height_;
  std::unique_ptr<ultralight::GPUDriverMetal> driver_;
  bool msaa_enabled_;
};
    
}  // namespace ultralight

