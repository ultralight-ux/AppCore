#if defined(DRIVER_D3D11)
#pragma once
#include "GPUContextD3D11.h"

namespace ultralight {

class GPUDriverD3D11;

class SwapChainD3D11 {
 public:
  SwapChainD3D11(GPUContextD3D11* context, GPUDriverD3D11* driver, HWND hWnd, int screen_width,
                 int screen_height, double screen_scale, bool fullscreen, bool enable_vsync,
                 bool sRGB, int samples);

  virtual ~SwapChainD3D11();

  virtual void PresentFrame();

  virtual void Resize(int width, int height);

  // This will be null if swap chain failed creation.
  virtual IDXGISwapChain* swap_chain();

  virtual ID3D11RenderTargetView* render_target_view();

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
  GPUContextD3D11* context_;
  GPUDriverD3D11* driver_;
  uint32_t render_buffer_id_;
  Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain_;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> back_buffer_view_;
  HWND hwnd_;
  double scale_;
  bool enable_vsync_;
  uint32_t width_;
  uint32_t height_;
  UINT back_buffer_width_;
  UINT back_buffer_height_;
  int samples_ = 1;
};

} // namespace ultralight

#endif
