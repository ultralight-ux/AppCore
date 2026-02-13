#if defined(DRIVER_D3D12)
#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include "D3D12MemAlloc.h"
#include "DescriptorAllocator.h"
#include <Ultralight/Geometry.h>
#include <Ultralight/platform/GPUDriver.h>

#define ENABLE_MSAA 1

using Microsoft::WRL::ComPtr;

namespace ultralight {

class SwapChainD3D12;

struct RenderState {
  RenderState();

  size_t Hash();

  void Reset();

  ShaderType shader_type;
  bool blend_enabled;
  uint8_t blend_src_factor;
  uint8_t blend_dst_factor;
  uint8_t blend_equation;
  DXGI_FORMAT render_target_format;
  UINT sample_count;
};

class GPUContextD3D12 {
public:
  GPUContextD3D12();

  virtual ~GPUContextD3D12();

  // Initialize device, allocator, command queue, root signature, shaders, and fence.
  // No HWND needed -- swap chains are created separately.
  bool Initialize();

  bool LoadShaders();

  virtual ID3D12Device* device();

  virtual ID3D12CommandQueue* command_queue() { return command_queue_.Get(); }

  virtual ID3D12GraphicsCommandList* command_list() { return command_list_.Get(); }

  D3D12MA::Allocator* allocator();

  DescriptorAllocator* descriptor_allocator() { return descriptor_allocator_.get(); }

  GPUDescriptorRingBuffer* gpu_descriptor_ring_buffer() { return gpu_descriptor_ring_buffer_.get(); }

  virtual void BeginDrawing();

  virtual void EndDrawing();

  // Execute the command list, signal fence, move to next frame.
  // Call this after all swap chains have been presented.
  virtual void Flush();

  virtual void set_shader_type(ShaderType type) { render_state_.shader_type = type; }
  virtual ShaderType shader_type() const { return render_state_.shader_type; }

  virtual void set_blend_enabled(bool val) { render_state_.blend_enabled = val; }
  virtual bool blend_enabled() const { return render_state_.blend_enabled; }

  // Dynamic blend state support (matching D3D11's interface)
  virtual void EnableBlend();
  virtual void DisableBlend();
  virtual void SetBlendState(uint8_t src_factor, uint8_t dst_factor, uint8_t equation);

  virtual void set_render_target_format(DXGI_FORMAT format) { render_state_.render_target_format = format; }
  virtual DXGI_FORMAT render_target_format() { return render_state_.render_target_format; }

  virtual void set_sample_count(UINT sample_count) { render_state_.sample_count = sample_count; }
  virtual UINT sample_count() { return render_state_.sample_count; }

  // This is calculated based on current RenderState
  ComPtr<ID3D12PipelineState> pipeline_state();

  void ReleaseWhenFrameComplete(ComPtr<D3D12MA::Allocation>&& resource);
  void ReleaseWhenFrameComplete(DescriptorHandle&& handle);

  // Multi-swap-chain support
  void AddSwapChain(SwapChainD3D12* swap_chain);
  void RemoveSwapChain(SwapChainD3D12* swap_chain);
  SwapChainD3D12* GetSwapChainByRenderBufferId(uint32_t render_buffer_id);

  UINT frame_index() { return frame_index_; }
  static const UINT FrameCount = 2;

  struct ConstantBuffer {
    ComPtr<D3D12MA::Allocation> buffer_;
    DescriptorHandle cbv_handle_;
  };

  ConstantBuffer* GetConstantBufferForWriting() { return frame().constant_buffer_pool_.GetBufferForWriting(this); }

private:
  void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
  void MoveToNextFrame();
  void WaitForGpu();
  void ResetCommandList();

  // The active frame index currently being drawn on the CPU (always < FrameCount)
  UINT frame_index_;

  // Fence to track which frames have been processed by GPU.
  HANDLE fence_event_;
  ComPtr<ID3D12Fence> fence_;

  struct ConstantBufferPool {
    std::vector<ConstantBuffer> pool_;
    size_t next_idx_ = 0;

    void Reset() { next_idx_ = 0; }

    // Gets a ConstantBuffer from the pool or creates one if none available.
    ConstantBuffer* GetBufferForWriting(GPUContextD3D12* ctx);
  };

  // Each Frame keeps track of its own allocator(s), resources, and handles.
  struct Frame {
    ComPtr<ID3D12CommandAllocator> command_allocator_;

    // All temporary resources (eg, UPLOAD HEAP) allocated during this frame.
    // These resources will be released after this frame has been processed on GPU.
    std::vector<ComPtr<D3D12MA::Allocation>> resources_to_release_;

    // All temporary descriptor handles allocated during this frame.
    // These handles will be released after this frame has been processed on GPU.
    std::vector<DescriptorHandle> handles_to_release_;

    // Fence value so we can track which frame has been processed by GPU.
    UINT64 fence_value_ = 0;

    // We use a per-frame pool of ConstantBuffers. The size of this pool currently never decreases.
    ConstantBufferPool constant_buffer_pool_;
  } frames_[FrameCount];

  Frame& frame() { return frames_[frame_index_]; }

  ComPtr<ID3D12Device> device_;

  ComPtr<ID3D12CommandQueue> command_queue_;
  ComPtr<ID3D12RootSignature> root_signature_;
  ComPtr<ID3D12GraphicsCommandList> command_list_;

  // Custom resource allocator, we use this to manage all resource allocations
  ComPtr<D3D12MA::Allocator> allocator_;

  // Custom CPU-only descriptor allocator, uses a freelist to recycle descriptors
  std::unique_ptr<DescriptorAllocator> descriptor_allocator_;

  // Ring buffer for passing descriptors to GPU. Uses a fixed-size heap.
  std::unique_ptr<GPUDescriptorRingBuffer> gpu_descriptor_ring_buffer_;

  struct ShaderEntry {
    D3D12_INPUT_LAYOUT_DESC input_layout;
    ComPtr<ID3DBlob> vertex_shader;
    ComPtr<ID3DBlob> pixel_shader;
  };
  typedef std::map<ShaderType, ShaderEntry> ShaderMap;
  ShaderMap shaders_;

  // We generate and cache pipeline states based on the active RenderState.
  RenderState render_state_;
  std::map<size_t, ComPtr<ID3D12PipelineState>> pipeline_states_;

  // Multi-swap-chain support
  std::unordered_map<uint32_t, SwapChainD3D12*> swap_chains_by_render_buffer_id_;

  D3D_FEATURE_LEVEL feature_level_;
};

}  // namespace ultralight

#endif
