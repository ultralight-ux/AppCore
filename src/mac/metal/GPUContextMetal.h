#pragma once
#import <MetalKit/MetalKit.h>
#import <Ultralight/platform/GPUDriver.h>
#import <Ultralight/platform/Config.h>
#include "GPUDriverImpl.h"
#include <memory>
#include <map>
#include <unordered_map>

namespace ultralight {
    
class GPUDriverMetal;
class WindowMac;
    
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
  GPUContextMetal(id<MTLDevice> device, bool enable_vsync, bool enable_msaa);
  
  virtual ~GPUContextMetal();
  
  // Inherited from GPUContext
  virtual ultralight::GPUDriverImpl* driver() const { return (GPUDriverImpl*)driver_.get(); }
  
  virtual ultralight::FaceWinding face_winding() const {
    return ultralight::FaceWinding::CounterClockwise;
  }
  
  virtual void BeginDrawing();
  
  virtual void EndDrawing();
  
  virtual void AddWindow(WindowMac* window);
  virtual void RemoveWindow(WindowMac* window);
  virtual WindowMac* GetWindowByRenderBufferId(uint32_t render_buffer_id);
  
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
  
  static const NSUInteger FrameCount = 3;
  
protected:
  dispatch_semaphore_t fence_;
  NSUInteger frame_index_ = 0;
  
  struct Frame {
  } frames_[FrameCount];
  
  id<MTLDevice> device_;
  id<MTLLibrary> library_;
  RenderState render_state_;
  std::map<size_t, id<MTLRenderPipelineState>> render_pipeline_states_;
  id<MTLCommandQueue> command_queue_;
  id<MTLCommandBuffer> command_buffer_;
  CAMetalLayer* layer_;
  std::unique_ptr<ultralight::GPUDriverMetal> driver_;
  bool msaa_enabled_;
  std::unordered_map<uint32_t, WindowMac*> windows_by_render_buffer_id_;
};
    
}  // namespace ultralight

