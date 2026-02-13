#if defined(DRIVER_D3D12)
#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <stdint.h>
#include "DescriptorAllocator.h"

using Microsoft::WRL::ComPtr;

namespace ultralight {

class GPUContextD3D12;
class GPUDriverD3D12;

class SwapChainD3D12 {
 public:
  SwapChainD3D12(GPUContextD3D12* context, GPUDriverD3D12* driver, HWND hWnd, int screen_width,
                 int screen_height, double screen_scale, bool fullscreen, bool enable_vsync,
                 bool sRGB, int samples);

  virtual ~SwapChainD3D12();

  virtual void PresentFrame();

  virtual void Resize(int width, int height);

  // This will be null if swap chain failed creation.
  virtual IDXGISwapChain3* swap_chain();

  // Current back buffer resource (for resource barriers)
  virtual ID3D12Resource* render_target();

  // Current back buffer RTV descriptor handle
  virtual D3D12_CPU_DESCRIPTOR_HANDLE render_target_view();

  // This will be 0 if swap chain failed creation.
  uint32_t render_buffer_id() const { return render_buffer_id_; }

  // Scale is calculated from monitor DPI, see Application::SetScale
  virtual void set_scale(double scale);
  virtual double scale() const;

  // This is in virtual units, not actual pixels.
  virtual void set_screen_size(uint32_t width, uint32_t height);
  virtual void screen_size(uint32_t& width, uint32_t& height);

  UINT back_buffer_width();
  UINT back_buffer_height();

 private:
  static const UINT BufferCount = 2;

  GPUContextD3D12* context_;
  GPUDriverD3D12* driver_;
  uint32_t render_buffer_id_;
  ComPtr<IDXGISwapChain3> swap_chain_;
  ComPtr<ID3D12Resource> render_targets_[BufferCount];
  DescriptorHandle rtv_handles_[BufferCount];
  HWND hwnd_;
  double scale_;
  bool enable_vsync_;
  uint32_t width_;
  uint32_t height_;
  UINT back_buffer_width_;
  UINT back_buffer_height_;
};

} // namespace ultralight

#endif
