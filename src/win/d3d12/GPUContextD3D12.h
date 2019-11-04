#if defined(DRIVER_D3D12)
#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <stdint.h>
#include <map>
#include <vector>
#include <memory>
#include <stdexcept>
#include "D3D12MemAlloc.h"
#include "DescriptorAllocator.h"
#include <Ultralight/Geometry.h>
#include <Ultralight/platform/GPUDriver.h>

namespace {

inline std::string HrToString(HRESULT hr)
{
  char s_str[64] = {};
  sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
  return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
  HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
  HRESULT Error() const { return m_hr; }
private:
  const HRESULT m_hr;
};

inline void ThrowIfFailed(HRESULT hr)
{
  if (FAILED(hr))
  {
    throw HrException(hr);
  }
}

}  // empty namespace

#define ENABLE_MSAA 1

using Microsoft::WRL::ComPtr;

namespace ultralight {

struct RenderState {
  RenderState();

  size_t Hash();

  void Reset();

  ShaderType shader_type;
  bool blend_enabled;
  DXGI_FORMAT render_target_format;
  UINT sample_count;
};

class GPUContextD3D12 {
public:
  GPUContextD3D12();

  virtual ~GPUContextD3D12();

  bool Initialize(HWND hWnd, int screen_width, int screen_height, double screen_scale, bool fullscreen, bool enable_vsync, bool sRGB, int samples);

  bool LoadShaders();

  virtual ID3D12Device* device();

  virtual ID3D12GraphicsCommandList* command_list() { return command_list_.Get(); }

  D3D12MA::Allocator* allocator();

  DescriptorAllocator* descriptor_allocator() { return descriptor_allocator_.get(); }

  GPUDescriptorRingBuffer* gpu_descriptor_ring_buffer() { return gpu_descriptor_ring_buffer_.get(); }

  D3D12_CPU_DESCRIPTOR_HANDLE current_rtv() { return frame().rtv_handle_.cpu_handle(); }

  virtual void BeginDrawing();

  virtual void EndDrawing();

  virtual void PresentFrame();

  virtual void Resize(int width, int height);

  virtual void set_shader_type(ShaderType type) { render_state_.shader_type = type; }
  virtual ShaderType shader_type() const { return render_state_.shader_type; }

  virtual void set_blend_enabled(bool val) { render_state_.blend_enabled = val; }
  virtual bool blend_enabled() const { return render_state_.blend_enabled; }

  virtual void set_render_target_format(DXGI_FORMAT format) { render_state_.render_target_format = format; }
  virtual DXGI_FORMAT render_target_format() { return render_state_.render_target_format; }

  virtual void set_sample_count(UINT sample_count) { render_state_.sample_count = sample_count; }
  virtual UINT sample_count() { return render_state_.sample_count; }

  // Scale is calculated from monitor DPI, see Application::SetScale
  virtual void set_scale(double scale);
  virtual double scale() const;

  // This is in virtual units, not actual pixels.
  virtual void set_screen_size(uint32_t width, uint32_t height);
  virtual void screen_size(uint32_t& width, uint32_t& height);

  UINT back_buffer_width();
  UINT back_buffer_height();
  
  // This is calculated based on current RenderState
  ComPtr<ID3D12PipelineState> pipeline_state();

  void ReleaseWhenFrameComplete(D3D12MA::ResourcePtr&& resource);
  void ReleaseWhenFrameComplete(DescriptorHandle&& handle);

  UINT frame_index() { return frame_index_; }
  static const UINT FrameCount = 3;

  struct ConstantBuffer {
    D3D12MA::ResourcePtr buffer_;
    DescriptorHandle cbv_handle_;
  };

  ConstantBuffer* GetConstantBufferForWriting() { return frame().constant_buffer_pool_.GetBufferForWriting(this); }

private:
  void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
  void MoveToNextFrame();
  void WaitForGpu();
  void ResetCommandList();
  
  HWND hwnd_;

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

  // Each Frame keeps track of its own backbuffer, allocator(s), resources, and handles.
  struct Frame {
    ComPtr<ID3D12CommandAllocator> command_allocator_;

    // The frame's backbuffer
    ComPtr<ID3D12Resource> render_target_;
    DescriptorHandle rtv_handle_;

    // All temporary resources (eg, UPLOAD HEAP) allocated during this frame.
    // These resources will be released after this frame has been processed on GPU.
    std::vector<D3D12MA::ResourcePtr> resources_to_release_;

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
  
  ComPtr<IDXGISwapChain3> swap_chain_;
  ComPtr<ID3D12CommandQueue> command_queue_;
  ComPtr<ID3D12RootSignature> root_signature_;
  ComPtr<ID3D12GraphicsCommandList> command_list_;

  // Custom resource allocator, we use this to manage all resource allocations
  D3D12MA::Allocator* allocator_ = nullptr;

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

  D3D_FEATURE_LEVEL feature_level_;
  double scale_;
  bool enable_vsync_;
  uint32_t width_;
  uint32_t height_;
  UINT back_buffer_width_;
  UINT back_buffer_height_;
  int samples_ = 1;
};

}  // namespace ultralight

#endif
