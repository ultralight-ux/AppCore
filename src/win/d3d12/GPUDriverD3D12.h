#if defined(DRIVER_D3D12)
#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <Ultralight/platform/GPUDriver.h>
#include <map>
#include <vector>
#include <memory>
#include "D3D12MemAlloc.h"
#include "GPUContextD3D12.h"

using Microsoft::WRL::ComPtr;

namespace ultralight {

class GPUContextD3D12;

/**
 * GPUDriver implementation for Direct3D 12.
 */
class GPUDriverD3D12 : public GPUDriver {
public:
  GPUDriverD3D12(GPUContextD3D12* context);
  virtual ~GPUDriverD3D12();

  virtual void BeginSynchronize() override;

  virtual void EndSynchronize() override;

  virtual uint32_t NextTextureId() override;

  virtual void CreateTexture(uint32_t texture_id,
    Ref<Bitmap> bitmap) override;

  virtual void UpdateTexture(uint32_t texture_id,
    Ref<Bitmap> bitmap) override;

  virtual void BindTexture(uint8_t texture_unit,
    uint32_t texture_id) override;

  virtual void DestroyTexture(uint32_t texture_id) override;

  virtual uint32_t NextRenderBufferId() override;

  virtual void CreateRenderBuffer(uint32_t render_buffer_id,
    const RenderBuffer& buffer) override;

  virtual void BindRenderBuffer(uint32_t render_buffer_id) override;

  virtual void ClearRenderBuffer(uint32_t render_buffer_id) override;

  virtual void DestroyRenderBuffer(uint32_t render_buffer_id) override;

  virtual uint32_t NextGeometryId() override;

  virtual void CreateGeometry(uint32_t geometry_id,
    const VertexBuffer& vertices,
    const IndexBuffer& indices) override;

  virtual void UpdateGeometry(uint32_t geometry_id,
    const VertexBuffer& vertices,
    const IndexBuffer& indices) override;

  virtual void DrawGeometry(uint32_t geometry_id,
    uint32_t indices_count,
    uint32_t indices_offset,
    const GPUState& state) override;

  virtual void DestroyGeometry(uint32_t geometry_id) override;

  virtual void UpdateCommandList(const CommandList& list) override;

  virtual bool HasCommandsPending() override;

  virtual void DrawCommandList() override;

  // Public Methods

  int batch_count() const { return batch_count_; }

protected:
  void BindGeometry(uint32_t id);
  void SetViewport(float width, float height);
  void UpdateConstantBuffer(const GPUState& state);
  void UpdateTextureResource(ID3D12Resource* resource, Ref<Bitmap> bitmap, bool is_msaa);
  Matrix ApplyProjection(const Matrix4x4& transform, float screen_width, float screen_height);

  GPUContextD3D12* context_;

  uint32_t next_texture_id_ = 1;
  uint32_t next_render_buffer_id_ = 1; // render buffer id 0 is reserved for default render target view.
  uint32_t next_geometry_id_ = 1;
  DescriptorHandle* cbv0_ = nullptr;
  DescriptorHandle* srv0_ = nullptr;
  DescriptorHandle* srv1_ = nullptr;

  struct GeometryEntry { 
	  VertexBufferFormat format;
    static const UINT num_frames = GPUContextD3D12::FrameCount;
    struct Frame {
      D3D12MA::ResourcePtr vertex_buffer;
      D3D12MA::ResourcePtr index_buffer;
      D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
      D3D12_INDEX_BUFFER_VIEW index_buffer_view;
    } frames[num_frames];
    UINT cur_frame = 0;
  };
  typedef std::map<uint32_t, GeometryEntry> GeometryMap;
  GeometryMap geometry_;

  struct TextureEntry { 
    D3D12MA::ResourcePtr texture;
    DescriptorHandle texture_srv_handle;

    // Whether or not the texture is currently D3D12_RESOURCE_STATE_RENDER_TARGET,
    // otherwise it is D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    bool is_bound_render_target;

    // These members are only used when MSAA is enabled
    bool is_msaa_render_target = false;
    bool needs_resolve = false;
    D3D12MA::ResourcePtr resolve_texture;
    DescriptorHandle resolve_texture_srv_handle;
  };

  typedef std::map<uint32_t, TextureEntry> TextureMap;
  TextureMap textures_;

  struct RenderTargetEntry {
    DescriptorHandle rtv_handle;
    uint32_t render_target_texture_id;
  };

  typedef std::map<uint32_t, RenderTargetEntry> RenderTargetMap;
  RenderTargetMap render_targets_;
  std::vector<Command> command_list_;
  int batch_count_;
};

}  // namespace ultralight

#endif
