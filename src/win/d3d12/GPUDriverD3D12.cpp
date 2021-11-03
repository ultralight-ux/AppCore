#if defined(DRIVER_D3D12)
#include "GPUDriverD3D12.h"
#include "GPUContextD3D12.h"
#include "d3dx12.h"
#include <directxcolors.h>
#include <d3dcompiler.h>
#include <string>
#include <sstream>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/FileSystem.h>
#include <AppCore/App.h>

namespace {

struct Vertex_2f_4ub_2f {
  DirectX::XMFLOAT2 pos;
  uint8_t color[4];
  DirectX::XMFLOAT2 obj;
};

struct Vertex_2f_4ub_2f_2f_28f {
  DirectX::XMFLOAT2 pos;
  uint8_t color[4];
  DirectX::XMFLOAT2 tex;
  DirectX::XMFLOAT2 obj;
  DirectX::XMFLOAT4 data_0;
  DirectX::XMFLOAT4 data_1;
  DirectX::XMFLOAT4 data_2;
  DirectX::XMFLOAT4 data_3;
  DirectX::XMFLOAT4 data_4;
  DirectX::XMFLOAT4 data_5;
  DirectX::XMFLOAT4 data_6;
};

struct Uniforms {
  DirectX::XMFLOAT4 State;
  DirectX::XMMATRIX Transform;
  DirectX::XMFLOAT4 Scalar4[2];
  DirectX::XMFLOAT4 Vector[8];
  uint32_t ClipSize;
  DirectX::XMMATRIX Clip[8];
};

HRESULT CompileShaderFromSource(const char* source, size_t source_size,
  const char* source_name, LPCSTR szEntryPoint,
  LPCSTR szShaderModel, ID3DBlob** ppBlobOut) {
  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
  dwShaderFlags |= D3DCOMPILE_DEBUG;
  dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#else
  dwShaderFlags |= D3DCOMPILE_PARTIAL_PRECISION;

  // Note that this may cause slow Application startup because the Shader Compiler
  // must perform heavy optimizations. In a production build you should use D3D's
  // HLSL Effect Compiler (fxc.exe) to compile the HLSL files offline which grants
  // near-instantaneous load times.
  dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL2;
#endif

  ComPtr<ID3DBlob> pErrorBlob;

  HRESULT hr = D3DCompile2(source, source_size, source_name, nullptr, nullptr,
    szEntryPoint, szShaderModel, dwShaderFlags, 0, 0, 0, 0, ppBlobOut,
    pErrorBlob.GetAddressOf());

  if (FAILED(hr) && pErrorBlob)
    OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));

  return hr;
}

HRESULT CompileShaderFromFile(const char* path, LPCSTR szEntryPoint,
  LPCSTR szShaderModel, ID3DBlob** ppBlobOut) {
  auto fs = ultralight::Platform::instance().file_system();

  if (!fs) {
    OutputDebugStringA("Could not load shaders, null FileSystem instance.");
    return -1;
  }

  ultralight::FileHandle handle = fs->OpenFile(path, false);

  if (handle == ultralight::invalidFileHandle) {
    OutputDebugStringA("Could not load shaders, file not found.");
    return -1;
  }

  int64_t file_size = 0;
  fs->GetFileSize(handle, file_size);

  std::unique_ptr<char[]> buffer(new char[file_size]);
  fs->ReadFromFile(handle, buffer.get(), file_size);

  return CompileShaderFromSource(buffer.get(), file_size, path, szEntryPoint,
    szShaderModel, ppBlobOut);
}

} // namespace (unnamed)

namespace ultralight {

GPUDriverD3D12::GPUDriverD3D12(GPUContextD3D12* context) : context_(context) {
}

GPUDriverD3D12::~GPUDriverD3D12() {
  while (geometry_.size())
    DestroyGeometry(geometry_.begin()->first);
  while (textures_.size())
    DestroyTexture(textures_.begin()->first);
  while (render_targets_.size())
    DestroyRenderBuffer(render_targets_.begin()->first);
}

void GPUDriverD3D12::CreateTexture(uint32_t texture_id,
  RefPtr<Bitmap> bitmap) {
  auto i = textures_.find(texture_id);
  if (i != textures_.end()) {
    MessageBoxW(nullptr,
      L"GPUDriverD3D12::CreateTexture, texture id already exists.", L"Error", MB_OK);
    return;
  }

  if (!(bitmap->format() == kBitmapFormat_BGRA8_UNORM_SRGB || bitmap->format() == kBitmapFormat_A8_UNORM)) {
    MessageBoxW(nullptr,
      L"GPUDriverD3D12::CreateTexture, unsupported format.", L"Error", MB_OK);
  }

  D3D12_RESOURCE_DESC resourceDesc = {};
  resourceDesc.MipLevels = 1;
  resourceDesc.Format = bitmap->format() == kBitmapFormat_BGRA8_UNORM_SRGB ? DXGI_FORMAT_B8G8R8A8_UNORM_SRGB : DXGI_FORMAT_A8_UNORM;
  resourceDesc.Width = bitmap->width();
  resourceDesc.Height = bitmap->height();
  resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
  resourceDesc.DepthOrArraySize = 1;
  resourceDesc.SampleDesc.Count = 1;
  resourceDesc.SampleDesc.Quality = 0;
  resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

  D3D12_CLEAR_VALUE clearValue = {};
  clearValue.Format = resourceDesc.Format;
  clearValue.Color[0] = 0.0f;
  clearValue.Color[1] = 0.0f;
  clearValue.Color[2] = 0.0f;
  clearValue.Color[3] = 0.0f;

  auto& entry = textures_[texture_id];

  if (bitmap->IsEmpty()) {
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
#if ENABLE_MSAA
    resourceDesc.SampleDesc.Count = 4;
    resourceDesc.SampleDesc.Quality = DXGI_STANDARD_MULTISAMPLE_QUALITY_PATTERN;
    entry.is_msaa_render_target = true;
#endif
  }

  D3D12MA::ALLOCATION_DESC allocationDesc = {};
  allocationDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
  D3D12MA::Allocation* alloc = nullptr;
  ThrowIfFailed(context_->allocator()->CreateResource(
    &allocationDesc,
    &resourceDesc,
    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
    bitmap->IsEmpty() ? &clearValue : nullptr,
    &alloc,
    __uuidof(ID3D12Resource),
    nullptr));
  entry.texture.reset(alloc);

  if (!bitmap->IsEmpty()) {
    UpdateTextureResource(entry.texture->GetResource(), bitmap, false);
  }
  else {
    RefPtr<Bitmap> zeroBitmap = Bitmap::Create(bitmap->width(), bitmap->height(), bitmap->format());
    zeroBitmap->Erase();
    UpdateTextureResource(entry.texture->GetResource(), zeroBitmap, entry.is_msaa_render_target);
  }

  // Describe and create a SRV for the texture.
  D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
  srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srvDesc.Format = resourceDesc.Format;
  srvDesc.ViewDimension = entry.is_msaa_render_target ? D3D12_SRV_DIMENSION_TEXTURE2DMS : D3D12_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;
  entry.texture_srv_handle = context_->descriptor_allocator()->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
  context_->device()->CreateShaderResourceView(entry.texture->GetResource(), &srvDesc, entry.texture_srv_handle.cpu_handle());
  entry.is_bound_render_target = false;

#if ENABLE_MSAA
  if (entry.is_msaa_render_target) {
    // Create resolve texture and shader resource view
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;

    alloc = nullptr;
    ThrowIfFailed(context_->allocator()->CreateResource(
      &allocationDesc,
      &resourceDesc,
      D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
      bitmap->IsEmpty() ? &clearValue : nullptr,
      &alloc,
      __uuidof(ID3D12Resource),
      nullptr));
    entry.resolve_texture.reset(alloc);

    RefPtr<Bitmap> zeroBitmap = Bitmap::Create(bitmap->width(), bitmap->height(), bitmap->format());
    zeroBitmap->Erase();
    UpdateTextureResource(entry.resolve_texture->GetResource(), zeroBitmap, false);

    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    entry.resolve_texture_srv_handle = context_->descriptor_allocator()->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    context_->device()->CreateShaderResourceView(entry.resolve_texture->GetResource(), &srvDesc, entry.resolve_texture_srv_handle.cpu_handle());
  }
#endif
}

void GPUDriverD3D12::UpdateTexture(uint32_t texture_id,
  RefPtr<Bitmap> bitmap) {
  auto i = textures_.find(texture_id);
  if (i == textures_.end()) {
    MessageBoxW(nullptr,
      L"GPUDriverD3D12::UpdateTexture, texture id doesn't exist.", L"Error", MB_OK);
    return;
  }

  auto& entry = i->second;
  UpdateTextureResource(entry.texture->GetResource(), bitmap, false);
}

void GPUDriverD3D12::DestroyTexture(uint32_t texture_id) {
  auto i = textures_.find(texture_id);
  if (i == textures_.end())
    return;

  context_->ReleaseWhenFrameComplete(std::move(i->second.texture));
  context_->ReleaseWhenFrameComplete(std::move(i->second.texture_srv_handle));
  context_->ReleaseWhenFrameComplete(std::move(i->second.resolve_texture));
  context_->ReleaseWhenFrameComplete(std::move(i->second.resolve_texture_srv_handle));
  textures_.erase(i);
}

void GPUDriverD3D12::CreateRenderBuffer(uint32_t render_buffer_id,
  const RenderBuffer& buffer) {
  if (render_buffer_id == 0) {
    MessageBoxW(nullptr,
      L"GPUDriverD3D12::CreateRenderBuffer, render buffer ID 0 is reserved for default render target view.", L"Error", MB_OK);
    return;
  }

  auto i = render_targets_.find(render_buffer_id);
  if (i != render_targets_.end()) {
    MessageBoxW(nullptr,
      L"GPUDriverD3D12::CreateRenderBuffer, render buffer id already exists.", L"Error", MB_OK);
    return;
  }

  auto tex_entry = textures_.find(buffer.texture_id);
  if (tex_entry == textures_.end()) {
    MessageBoxW(nullptr,
      L"GPUDriverD3D12::CreateRenderBuffer, texture id doesn't exist.", L"Error", MB_OK);
    return;
  }

  auto& render_target_entry = render_targets_[render_buffer_id];
  render_target_entry.rtv_handle = context_->descriptor_allocator()->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  context_->device()->CreateRenderTargetView(tex_entry->second.texture->GetResource(), nullptr, render_target_entry.rtv_handle.cpu_handle());
  render_target_entry.render_target_texture_id = buffer.texture_id;
}

void GPUDriverD3D12::CreateGeometry(uint32_t geometry_id,
  const VertexBuffer& vertices,
  const IndexBuffer& indices) {
  if (geometry_.find(geometry_id) != geometry_.end())
    return;

  GeometryEntry& geometry = geometry_[geometry_id];
  geometry.format = vertices.format;

  D3D12MA::ALLOCATION_DESC allocationDesc = {};
  allocationDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

  for (UINT i = 0; i < GeometryEntry::num_frames; ++i) {
    GeometryEntry::Frame& frame = geometry.frames[i];

    D3D12MA::Allocation* alloc = nullptr;
    ThrowIfFailed(context_->allocator()->CreateResource(
      &allocationDesc,
      &CD3DX12_RESOURCE_DESC::Buffer(vertices.size),
      D3D12_RESOURCE_STATE_GENERIC_READ,
      NULL,
      &alloc,
      __uuidof(ID3D12Resource),
      nullptr));

    // Copy vertex data for current frame
    if (i == geometry.cur_frame) {
      UINT8* pData;
      CD3DX12_RANGE readRange(0, 0);
      ThrowIfFailed(alloc->GetResource()->Map(0, &readRange, reinterpret_cast<void**>(&pData)));
      memcpy(pData, vertices.data, vertices.size);
      alloc->GetResource()->Unmap(0, nullptr);
    }

    UINT stride = 0;
    if (vertices.format == kVertexBufferFormat_2f_4ub_2f)
      stride = sizeof(Vertex_2f_4ub_2f);
    else
      stride = sizeof(Vertex_2f_4ub_2f_2f_28f);

    // Initialize the vertex buffer view.
    frame.vertex_buffer_view.BufferLocation = alloc->GetResource()->GetGPUVirtualAddress();
    frame.vertex_buffer_view.StrideInBytes = stride;
    frame.vertex_buffer_view.SizeInBytes = vertices.size;

    // Take ownership of alloc (vertex buffer resource)
    frame.vertex_buffer.reset(alloc);

    // Create index buffer allocation
    alloc = nullptr;
    ThrowIfFailed(context_->allocator()->CreateResource(
      &allocationDesc,
      &CD3DX12_RESOURCE_DESC::Buffer(indices.size),
      D3D12_RESOURCE_STATE_GENERIC_READ,
      NULL,
      &alloc,
      __uuidof(ID3D12Resource),
      nullptr));

    // Copy index data for current frame
    if (i == geometry.cur_frame) {
      UINT8* pData;
      CD3DX12_RANGE readRange(0, 0);
      ThrowIfFailed(alloc->GetResource()->Map(0, &readRange, reinterpret_cast<void**>(&pData)));
      memcpy(pData, indices.data, indices.size);
      alloc->GetResource()->Unmap(0, nullptr);
    }

    // Initialize the index buffer view.
    frame.index_buffer_view.BufferLocation = alloc->GetResource()->GetGPUVirtualAddress();
    frame.index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
    frame.index_buffer_view.SizeInBytes = indices.size;

    // Take ownership of alloc (index buffer resource)
    frame.index_buffer.reset(alloc);
  }
}

void GPUDriverD3D12::UpdateGeometry(uint32_t geometry_id,
  const VertexBuffer& vertices,
  const IndexBuffer& indices) {
  auto i = geometry_.find(geometry_id);
  if (i == geometry_.end()) {
    MessageBoxW(nullptr,
      L"GPUDriverD3D12::UpdateGeometry, geometry id doesn't exist.", L"Error", MB_OK);
    return;
  }

  GeometryEntry& geometry = i->second;

  // Iterate current geometry frame, we buffer geometry uploads per-frame
  geometry.cur_frame = (geometry.cur_frame + 1) % GeometryEntry::num_frames;
  
  GeometryEntry::Frame& frame = geometry.frames[geometry.cur_frame];

  // Copy vertex data
  {
    ID3D12Resource* resource = frame.vertex_buffer->GetResource();
    UINT8* pData;
    CD3DX12_RANGE readRange(0, 0);
    ThrowIfFailed(resource->Map(0, &readRange, reinterpret_cast<void**>(&pData)));
    memcpy(pData, vertices.data, vertices.size);
    resource->Unmap(0, nullptr);
  }

  // Copy index data
  {
    ID3D12Resource* resource = frame.index_buffer->GetResource();
    UINT8* pData;
    CD3DX12_RANGE readRange(0, 0);
    ThrowIfFailed(resource->Map(0, &readRange, reinterpret_cast<void**>(&pData)));
    memcpy(pData, indices.data, indices.size);
    resource->Unmap(0, nullptr);
  }
}

void GPUDriverD3D12::DestroyGeometry(uint32_t geometry_id) {
  auto i = geometry_.find(geometry_id);
  if (i == geometry_.end())
    return;

  for (UINT id = 0; id < GPUContextD3D12::FrameCount; ++id) {
    GeometryEntry::Frame& frame = i->second.frames[id];
    if (frame.vertex_buffer) {
      context_->ReleaseWhenFrameComplete(std::move(frame.vertex_buffer));
      context_->ReleaseWhenFrameComplete(std::move(frame.index_buffer));
    }
  }

  geometry_.erase(i);
}

// Inherited from GPUDriverImpl:


void GPUDriverD3D12::BindTexture(uint8_t texture_unit,
  uint32_t texture_id) {
  auto i = textures_.find(texture_id);
  if (i == textures_.end()) {
    MessageBoxW(nullptr,
      L"GPUDriverD3D12::BindTexture, texture id doesn't exist.", L"Error", MB_OK);
    return;
  }

  if (i->second.is_bound_render_target) {
    // Signal the resource that we are transitioning it for use in shader
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(i->second.texture->GetResource(),
      D3D12_RESOURCE_STATE_RENDER_TARGET,
      D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0);
    context_->command_list()->ResourceBarrier(1, &barrier);
    i->second.is_bound_render_target = false;
  }

  if (i->second.is_msaa_render_target) {
    // Signal both resources that we are transitioning into MSAA resolve operation
    {
      D3D12_RESOURCE_BARRIER barriers[2] = {
          CD3DX12_RESOURCE_BARRIER::Transition(i->second.texture->GetResource(),
              D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RESOLVE_SOURCE),
          CD3DX12_RESOURCE_BARRIER::Transition(i->second.resolve_texture->GetResource(),
              D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RESOLVE_DEST)
      };
      context_->command_list()->ResourceBarrier(2, barriers);
    }

    context_->command_list()->ResolveSubresource(i->second.resolve_texture->GetResource(), 0, i->second.texture->GetResource(), 0, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);
    i->second.needs_resolve = false;

    // Signal both resources that we are transitioning for use in shader
    {
      D3D12_RESOURCE_BARRIER barriers[2] = {
          CD3DX12_RESOURCE_BARRIER::Transition(i->second.texture->GetResource(),
              D3D12_RESOURCE_STATE_RESOLVE_SOURCE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE),
          CD3DX12_RESOURCE_BARRIER::Transition(i->second.resolve_texture->GetResource(),
              D3D12_RESOURCE_STATE_RESOLVE_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
      };
      context_->command_list()->ResourceBarrier(2, barriers);
    }
  }

  DescriptorHandle* texture_handle = nullptr;
  if (i->second.is_msaa_render_target)
    texture_handle = &i->second.resolve_texture_srv_handle;
  else
    texture_handle = &i->second.texture_srv_handle;

  // Cache handle so we can bind to descriptor table later in DrawGeometry
  if (texture_unit == 0)
    srv0_ = texture_handle;
  else
    srv1_ = texture_handle;
}


void GPUDriverD3D12::BindRenderBuffer(uint32_t render_buffer_id) {
  D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = { 0 };
  UINT sample_count = 1;
  if (render_buffer_id == 0) {
    rtv_handle = context_->current_rtv();
    context_->set_render_target_format(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
  }
  else {
    auto i = render_targets_.find(render_buffer_id);
    if (i == render_targets_.end()) {
      MessageBoxW(nullptr,
        L"GPUDriverD3D12::BindRenderBuffer, render buffer id doesn't exist.", L"Error", MB_OK);
      return;
    }

    rtv_handle = i->second.rtv_handle.cpu_handle();
    context_->set_render_target_format(DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);

    // We need to transition the resource state to a render target
    auto tex_entry = textures_.find(i->second.render_target_texture_id);
    if (tex_entry == textures_.end()) {
      MessageBoxW(nullptr,
        L"GPUDriverD3D12::BindRenderBuffer, texture id doesn't exist.", L"Error", MB_OK);
      return;
    }

    // Signal the resource that we are transitioning it for use as a render target
    if (!tex_entry->second.is_bound_render_target) {
      auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(tex_entry->second.texture->GetResource(),
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET, 0);
      context_->command_list()->ResourceBarrier(1, &barrier);
      tex_entry->second.is_bound_render_target = true;
    }

#if ENABLE_MSAA
    if (tex_entry->second.is_msaa_render_target) {
      tex_entry->second.needs_resolve = true;
      sample_count = 4;
    }
#endif
  }

  context_->set_sample_count(sample_count);
  context_->command_list()->OMSetRenderTargets(1, &rtv_handle, false, nullptr);
}

void GPUDriverD3D12::ClearRenderBuffer(uint32_t render_buffer_id) {
  float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

  if (render_buffer_id == 0) {
    context_->command_list()->ClearRenderTargetView(context_->current_rtv(), color, 0, nullptr);
    return;
  }

  auto i = render_targets_.find(render_buffer_id);
  if (i == render_targets_.end()) {
    MessageBoxW(nullptr,
      L"GPUDriverD3D12::ClearRenderBuffer, render buffer id doesn't exist.", L"Error", MB_OK);
    return;
  }

  // We need to transition the resource state to a render target
  auto tex_entry = textures_.find(i->second.render_target_texture_id);
  if (tex_entry == textures_.end()) {
    MessageBoxW(nullptr,
      L"GPUDriverD3D12::BindRenderBuffer, texture id doesn't exist.", L"Error", MB_OK);
    return;
  }

  // Signal the resource that we are transitioning it for use as a render target
  if (!tex_entry->second.is_bound_render_target) {
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(tex_entry->second.texture->GetResource(),
      D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET, 0);
    context_->command_list()->ResourceBarrier(1, &barrier);
    tex_entry->second.is_bound_render_target = true;
  }

#if ENABLE_MSAA
  if (tex_entry->second.is_msaa_render_target)
    tex_entry->second.needs_resolve = true;
#endif

  context_->command_list()->ClearRenderTargetView(i->second.rtv_handle.cpu_handle(), color, 0, nullptr);
}

void GPUDriverD3D12::DestroyRenderBuffer(uint32_t render_buffer_id) {
  auto i = render_targets_.find(render_buffer_id);
  if (i == render_targets_.end())
    return;

  context_->ReleaseWhenFrameComplete(std::move(i->second.rtv_handle));
  render_targets_.erase(i);
}


void GPUDriverD3D12::DrawGeometry(uint32_t geometry_id,
  uint32_t indices_count,
  uint32_t indices_offset,
  const GPUState& state) {
  cbv0_ = nullptr;
  srv0_ = nullptr;
  srv1_ = nullptr;

  BindRenderBuffer(state.render_buffer_id);

  SetViewport((float)state.viewport_width, (float)state.viewport_height);

  if (state.texture_1_id)
    BindTexture(0, state.texture_1_id);

  if (state.texture_2_id)
    BindTexture(1, state.texture_2_id);

  UpdateConstantBuffer(state);

  BindGeometry(geometry_id);

  context_->set_shader_type((ShaderType)state.shader_type);

  context_->set_blend_enabled(state.enable_blend);

  if (state.enable_scissor) {
    CD3DX12_RECT scissor_rect(
      (LONG)(state.scissor_rect.left),
      (LONG)(state.scissor_rect.top),
      (LONG)(state.scissor_rect.right),
      (LONG)(state.scissor_rect.bottom));
    context_->command_list()->RSSetScissorRects(1, &scissor_rect);
  }
  else {
    CD3DX12_RECT scissor_rect(
      (LONG)(0),
      (LONG)(0),
      (LONG)(state.viewport_width),
      (LONG)(state.viewport_height));
    context_->command_list()->RSSetScissorRects(1, &scissor_rect);
  }

  D3D12_GPU_DESCRIPTOR_HANDLE table = context_->gpu_descriptor_ring_buffer()->StoreTable(cbv0_, srv0_, srv1_);
  context_->command_list()->SetGraphicsRootDescriptorTable(0, table);

  context_->command_list()->SetPipelineState(context_->pipeline_state().Get());
  context_->command_list()->DrawIndexedInstanced(indices_count, 1, indices_offset, 0, 0);
  batch_count_++;
}

// Protected methods:

void GPUDriverD3D12::BindGeometry(uint32_t id) {
  auto i = geometry_.find(id);
  if (i == geometry_.end())
    return;

  GeometryEntry::Frame& geometry_frame = i->second.frames[i->second.cur_frame];

  context_->command_list()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  context_->command_list()->IASetVertexBuffers(0, 1, &geometry_frame.vertex_buffer_view);
  context_->command_list()->IASetIndexBuffer(&geometry_frame.index_buffer_view);
}

void GPUDriverD3D12::SetViewport(float width, float height) {
  CD3DX12_VIEWPORT viewport(0.0f, 0.0f, width, height);
  context_->command_list()->RSSetViewports(1, &viewport);
}

void GPUDriverD3D12::UpdateConstantBuffer(const GPUState& state) {
  Matrix model_view_projection = ApplyProjection(state.transform, (float)state.viewport_width, (float)state.viewport_height);

  Uniforms uniforms;
  uniforms.State = { state.enable_snap ? 1.0f : 0.0f, (float)state.viewport_width, (float)state.viewport_height, (float)context_->scale() };
  uniforms.Transform = DirectX::XMMATRIX(model_view_projection.GetMatrix4x4().data);
  uniforms.Scalar4[0] =
  { state.uniform_scalar[0], state.uniform_scalar[1], state.uniform_scalar[2], state.uniform_scalar[3] };
  uniforms.Scalar4[1] =
  { state.uniform_scalar[4], state.uniform_scalar[5], state.uniform_scalar[6], state.uniform_scalar[7] };
  for (size_t i = 0; i < 8; ++i)
    uniforms.Vector[i] = DirectX::XMFLOAT4(state.uniform_vector[i].value);
  uniforms.ClipSize = state.clip_size;
  for (size_t i = 0; i < state.clip_size; ++i)
    uniforms.Clip[i] = DirectX::XMMATRIX(state.clip[i].data);

  // Get a constant buffer for writing
  GPUContextD3D12::ConstantBuffer* cb = context_->GetConstantBufferForWriting();

  // Update data
  {
    ID3D12Resource* resource = cb->buffer_->GetResource();
    UINT8* pData;
    CD3DX12_RANGE readRange(0, 0);
    ThrowIfFailed(resource->Map(0, &readRange, reinterpret_cast<void**>(&pData)));
    memcpy(pData, &uniforms, sizeof(Uniforms));
    resource->Unmap(0, nullptr);
  }

  // Cache handle so we can bind to descriptor table later in DrawGeometry
  cbv0_ = &cb->cbv_handle_;
}

void GPUDriverD3D12::UpdateTextureResource(ID3D12Resource* resource, RefPtr<Bitmap> bitmap, bool is_msaa) {
  // Ignore requests to update MSAA texture resources
  if (is_msaa)
    return;

  if (bitmap->IsEmpty()) {
    MessageBoxW(nullptr,
      L"GPUDriverD3D12::UpdateTextureResource, bitmap is empty.", L"Error", MB_OK);
    return;
  }

  context_->command_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST));

  const UINT64 uploadBufferSize = GetRequiredIntermediateSize(resource, 0, 1);

  D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

  D3D12MA::Allocation* upload_texture;
  D3D12MA::ALLOCATION_DESC allocationDesc = {};
  allocationDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
  ThrowIfFailed(context_->allocator()->CreateResource(
    &allocationDesc,
    &desc,
    D3D12_RESOURCE_STATE_GENERIC_READ,
    NULL,
    &upload_texture,
    __uuidof(ID3D12Resource),
    nullptr));

  D3D12_SUBRESOURCE_DATA textureData = {};
  textureData.pData = bitmap->LockPixels();
  textureData.RowPitch = bitmap->row_bytes();
  textureData.SlicePitch = textureData.RowPitch * bitmap->height();
  UpdateSubresources(context_->command_list(), resource, upload_texture->GetResource(), 0, 0, 1, &textureData);
  bitmap->UnlockPixels();

  context_->command_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

  D3D12MA::ResourcePtr upload_texture_ptr;
  upload_texture_ptr.reset(upload_texture);
  context_->ReleaseWhenFrameComplete(std::move(upload_texture_ptr));
}

Matrix GPUDriverD3D12::ApplyProjection(const Matrix4x4& transform, float screen_width, float screen_height) {
  Matrix transform_mat;
  transform_mat.Set(transform);

  Matrix result;
  result.SetOrthographicProjection(screen_width, screen_height, false);
  result.Transform(transform_mat);

  return result;
}

}  // namespace ultralight

#endif
