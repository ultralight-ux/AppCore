#pragma once
#import <MetalKit/MetalKit.h>
#import <Ultralight/platform/GPUDriver.h>
#import <Ultralight/platform/Config.h>
#include "GPUDriverImpl.h"
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>

namespace ultralight {

class GPUDriverMetal;
class WindowMac;

struct RenderState {
  RenderState();

  size_t Hash();

  ShaderType shader_type;
  bool blend_enabled;
  uint8_t blend_src_factor;
  uint8_t blend_dst_factor;
  uint8_t blend_equation;
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

  void SetBlendState(uint8_t src_factor, uint8_t dst_factor, uint8_t equation);
  void DisableBlend();
  
  virtual void set_pixel_format(MTLPixelFormat format) { render_state_.pixel_format = format; }
  virtual MTLPixelFormat pixel_format() { return render_state_.pixel_format; }
  
  virtual void set_sample_count(NSUInteger count) { render_state_.sample_count = count; }
  virtual NSUInteger sample_count() { return render_state_.sample_count; }
  
  // Inherited from PlatformGPUContext
  virtual id<MTLDevice> device() { return device_; }
  virtual id<MTLSamplerState> sampler_state() { return sampler_state_; }
  virtual id<MTLRenderPipelineState> render_pipeline_state();

  virtual id<MTLCommandBuffer> command_buffer();
  virtual void CommitCommandBuffer();
  
  // Clear pipeline support
  virtual id<MTLRenderPipelineState> GetClearPipelineState(MTLPixelFormat format);

  // Get a uniform buffer from the current frame's pool (reused across frames)
  id<MTLBuffer> GetUniformBuffer(size_t size);

  static const NSUInteger FrameCount = 3;

protected:
  void LoadShaders();

protected:
  dispatch_semaphore_t fence_;
  NSUInteger frame_index_ = 0;

  struct UniformBufferPool {
    std::vector<id<MTLBuffer>> pool_;
    size_t next_idx_ = 0;

    void Reset() { next_idx_ = 0; }
    id<MTLBuffer> GetBufferForWriting(id<MTLDevice> device, size_t size);
  };

  struct Frame {
    UniformBufferPool uniform_buffer_pool_;
  } frames_[FrameCount];
  
  id<MTLDevice> device_;
  id<MTLLibrary> library_;
  RenderState render_state_;
  std::map<size_t, id<MTLRenderPipelineState>> render_pipeline_states_;
  
  // Individual shader libraries for new system
  std::map<ShaderType, std::pair<id<MTLLibrary>, id<MTLLibrary>>> shader_libraries_;
  
  // Clear pipeline state support
  id<MTLLibrary> clear_vertex_library_;
  id<MTLLibrary> clear_fragment_library_;
  std::map<MTLPixelFormat, id<MTLRenderPipelineState>> clear_pipeline_states_;
  
  id<MTLCommandQueue> command_queue_;
  id<MTLCommandBuffer> command_buffer_;
  id<MTLSamplerState> sampler_state_;
  CAMetalLayer* layer_;
  std::unique_ptr<ultralight::GPUDriverMetal> driver_;
  bool msaa_enabled_;
  std::unordered_map<uint32_t, WindowMac*> windows_by_render_buffer_id_;
};
    
}  // namespace ultralight

