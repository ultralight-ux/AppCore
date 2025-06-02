#pragma once
// Include generated Metal shader headers
#include "vertex_path_vs.h"
#include "vertex_quad_vs.h" 
#include "fill_ps.h"
#include "fill_path_ps.h"
#include "filter_basic_ps.h"
#include "filter_blur_ps.h"
#include "filter_dropshadow_ps.h"
#import "GPUContextMetal.h"
#include "GPUDriverImpl.h"
#import <MetalKit/MetalKit.h>
#include <map>
#include <mutex>
#import <simd/simd.h>
#include <vector>

namespace ultralight {

class GPUContextMetal;

class GPUDriverMetal : public GPUDriverImpl {
public:
    GPUDriverMetal(GPUContextMetal* context);
    virtual ~GPUDriverMetal();

    // Inherited from GPUDriver:

    virtual void CreateTexture(uint32_t texture_id,
        RefPtr<Bitmap> bitmap) override;

    virtual void UpdateTexture(uint32_t texture_id,
        RefPtr<Bitmap> bitmap) override;

    virtual void DestroyTexture(uint32_t texture_id) override;

    virtual void CreateRenderBuffer(uint32_t render_buffer_id,
        const RenderBuffer& buffer) override;

    virtual void DestroyRenderBuffer(uint32_t render_buffer_id) override;

    virtual void CreateGeometry(uint32_t geometry_id,
        const VertexBuffer& vertices,
        const IndexBuffer& indices) override;

    virtual void UpdateGeometry(uint32_t geometry_id,
        const VertexBuffer& vertices,
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

protected:
    void SetGPUState(const GPUState& state);

    Matrix ApplyProjection(const Matrix4x4& transform, float screen_width, float screen_height);

    void ApplyScissor(const GPUState& state);

    void UpdateTextureResource(id<MTLTexture> texture, RefPtr<Bitmap> bitmap);

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

    typedef std::map<uint32_t, TextureEntry> TextureMap;
    TextureMap textures_;
    std::mutex texture_mutex_;

    typedef std::map<uint32_t, RenderBuffer> RenderBufferMap;
    RenderBufferMap render_buffers_;

    struct GeometryEntry {
        id<MTLBuffer> vertex_buffer;
        id<MTLBuffer> index_buffer;
    };

    void UpdateGeometryResource(GeometryEntry& geometry, const VertexBuffer& vertices,
        const IndexBuffer& indices);

    typedef std::map<uint32_t, GeometryEntry> GeometryMap;
    GeometryMap geometry_;
    std::mutex geometry_mutex_;

    bool is_drawing_to_window_ = false;
};

} // namespace ultralight
