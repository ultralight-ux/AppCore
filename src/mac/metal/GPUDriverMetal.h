#pragma once
#import <MetalKit/MetalKit.h>
#import <simd/simd.h>
#import "../../../shaders/metal/src/ShaderTypes.h"
#include <Ultralight/platform/GPUDriver.h>
#include <map>
#include <vector>
#include <atomic>

namespace ultralight {
    
class GPUContextMetal;

class GPUDriverMetal : public GPUDriver {
public:
    GPUDriverMetal(GPUContextMetal* context);
    virtual ~GPUDriverMetal();
    
    // Synchronization
    
    virtual void BeginSynchronize();
    
    virtual void EndSynchronize();
    
    // Textures
    
    virtual uint32_t NextTextureId() { return next_texture_id_++; }
    
    virtual void CreateTexture(uint32_t texture_id,
                               Ref<Bitmap> bitmap);
    
    virtual void UpdateTexture(uint32_t texture_id,
                               Ref<Bitmap> bitmap);
    
    virtual void BindTexture(uint8_t texture_unit,
                             uint32_t texture_id);
    
    virtual void DestroyTexture(uint32_t texture_id);
    
    // Offscreen Rendering
    
    virtual uint32_t NextRenderBufferId() { return next_render_buffer_id_++; }
    
    virtual void CreateRenderBuffer(uint32_t render_buffer_id,
                                    const RenderBuffer& buffer);
    
    virtual void BindRenderBuffer(uint32_t render_buffer_id);
    
    virtual void ClearRenderBuffer(uint32_t render_buffer_id);
    
    virtual void DestroyRenderBuffer(uint32_t render_buffer_id);
    
    // Geometry
    
    virtual uint32_t NextGeometryId() { return next_geometry_id_++; }
    
    virtual void CreateGeometry(uint32_t geometry_id,
                                const VertexBuffer& vertices,
                                const IndexBuffer& indices);
    
    virtual void UpdateGeometry(uint32_t geometry_id,
                                const VertexBuffer& buffer,
                                const IndexBuffer& indices);
    
    
    virtual void DrawGeometry(uint32_t geometry_id,
                              uint32_t indices_count,
                              uint32_t indices_offset,
                              const GPUState& state);
    
    
    virtual void DestroyGeometry(uint32_t geometry_id);
    
    // Command Queue
    
    virtual void UpdateCommandList(const CommandList& queue);
    
    virtual bool HasCommandsPending();
    
    virtual void DrawCommandList();
    
    void EndDrawing();
    
protected:
    // We double-buffer vertex/index geometry buffers
    static const NSUInteger RingBufferSize = 1;
    
    template <typename T>
    class RingBuffer {
    public:
        RingBuffer() {}
        T& current() { return buf_[idx_]; }
        void iterate() { idx_ = (idx_ + 1) % RingBufferSize; }
    protected:
        T buf_[RingBufferSize];
        size_t idx_ = 0;
    };
    
    void SetGPUState(const GPUState& state);
    
    Matrix ApplyProjection(const Matrix4x4& transform, float screen_width, float screen_height);
    
    void ApplyScissor(const GPUState& state);
    
    GPUContextMetal* context_;
    
    // The render command encoder only exists during rendering
    id<MTLRenderCommandEncoder> render_encoder_;
    uint32_t render_encoder_render_buffer_id_;
    NSUInteger render_encoder_render_buffer_width_;
    NSUInteger render_encoder_render_buffer_height_;
    
    uint32_t next_texture_id_ = 1;
    uint32_t next_render_buffer_id_ = 1; // 0 is reserved for screen
    uint32_t next_geometry_id_ = 1;
    
    struct Texture {
        id<MTLTexture> texture_;
        id<MTLTexture> resolve_texture_;
        bool needs_init_ = true;
    };
    
    //typedef std::map<uint32_t, RingBuffer<Texture>> TextureMap;
    typedef std::map<uint32_t, Texture> TextureMap;
    TextureMap textures_;
    
    typedef std::map<uint32_t, RenderBuffer> RenderBufferMap;
    RenderBufferMap render_buffers_;
    
    struct GeometryBuffer {
        id<MTLBuffer> vertex_buffer_;
        id<MTLBuffer> index_buffer_;
        int64_t owning_frame_id_ = 0;
    };
    
    typedef std::map<uint32_t, RingBuffer<GeometryBuffer>> GeometryMap;
    GeometryMap geometry_;
    
    int64_t cpu_frame_id_ = 1;
    std::atomic<int64_t> gpu_frame_id_;
    
    std::vector<Command> command_list_;
};
    
} // namespace ultralight

