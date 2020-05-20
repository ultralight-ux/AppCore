#pragma once
#import <MetalKit/MetalKit.h>
#import <simd/simd.h>
#import "../../../shaders/metal/src/ShaderTypes.h"
#include "GPUDriverImpl.h"
#include <map>
#include <vector>
#import "GPUContextMetal.h"

namespace ultralight {
    
class GPUContextMetal;

class GPUDriverMetal : public GPUDriverImpl {
 public:
  GPUDriverMetal(GPUContextMetal* context);
  virtual ~GPUDriverMetal();
  
  // Inherited from GPUDriver:
  
  virtual void CreateTexture(uint32_t texture_id,
                             Ref<Bitmap> bitmap) override;
  
  virtual void UpdateTexture(uint32_t texture_id,
                             Ref<Bitmap> bitmap) override;
  

  virtual void DestroyTexture(uint32_t texture_id) override;

  virtual void CreateRenderBuffer(uint32_t render_buffer_id,
                                  const RenderBuffer& buffer) override;
  
  virtual void DestroyRenderBuffer(uint32_t render_buffer_id) override;
  
  virtual void CreateGeometry(uint32_t geometry_id,
                              const VertexBuffer& vertices,
                              const IndexBuffer& indices) override;
  
  virtual void UpdateGeometry(uint32_t geometry_id,
                              const VertexBuffer& buffer,
                              const IndexBuffer& indices) override;
  
  virtual void DestroyGeometry(uint32_t geometry_id) override;
  
  // Inherited from GPUDriverImpl:
  
  virtual const char* name() override { return "Metal"; }
  
  virtual void BeginDrawing() override {}
  
  virtual void EndDrawing() override;
  
  virtual void BindTexture(uint8_t texture_unit,
                           uint32_t texture_id) override;
  
  virtual void BindRenderBuffer(uint32_t render_buffer_id) override;
  
  virtual void ClearRenderBuffer(uint32_t render_buffer_id) override;
  
  virtual void DrawGeometry(uint32_t geometry_id,
                            uint32_t indices_count,
                            uint32_t indices_offset,
                            const GPUState& state) override;
  
  // Public members:
  
  void set_drawable_needs_flush(bool val) { drawable_needs_flush_ = val; }
  
protected:
  void SetGPUState(const GPUState& state);
  
  Matrix ApplyProjection(const Matrix4x4& transform, float screen_width, float screen_height);
  
  void ApplyScissor(const GPUState& state);
  
  void UpdateTextureResource(id<MTLTexture> texture, Ref<Bitmap> bitmap);
  
  GPUContextMetal* context_;
  
  // The render command encoder only exists during rendering
  id<MTLRenderCommandEncoder> render_encoder_;
  uint32_t render_encoder_render_buffer_id_;
  NSUInteger render_encoder_render_buffer_width_;
  NSUInteger render_encoder_render_buffer_height_;
  
  // The blit command encoder only exists during rendering
  id<MTLBlitCommandEncoder> blit_encoder_;
  
  struct TextureEntry {
    id<MTLTexture> texture_;
    id<MTLTexture> resolve_texture_;
    bool needs_init_ = true;
  };
  
  bool drawable_needs_flush_ = false;
  
  typedef std::map<uint32_t, TextureEntry> TextureMap;
  TextureMap textures_;
  
  typedef std::map<uint32_t, RenderBuffer> RenderBufferMap;
  RenderBufferMap render_buffers_;
  
  struct GeometryEntry {
    static const NSUInteger num_frames = GPUContextMetal::FrameCount;
    struct Frame {
      id<MTLBuffer> vertex_buffer;
      id<MTLBuffer> index_buffer;
    } frames[num_frames];
    NSUInteger cur_frame = 0;
  };
  
  typedef std::map<uint32_t, GeometryEntry> GeometryMap;
  GeometryMap geometry_;
};
    
} // namespace ultralight

