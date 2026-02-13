#if defined(DRIVER_D3D12)
#include "SwapChainD3D12.h"
#include "GPUContextD3D12.h"
#include "GPUDriverD3D12.h"
#include "d3dx12.h"
#include <cassert>

namespace ultralight {

SwapChainD3D12::SwapChainD3D12(GPUContextD3D12* context, GPUDriverD3D12* driver, HWND hWnd,
                               int screen_width, int screen_height, double screen_scale,
                               bool fullscreen, bool enable_vsync, bool sRGB, int samples)
    : context_(context), driver_(driver), hwnd_(hWnd), enable_vsync_(enable_vsync),
      render_buffer_id_(0), back_buffer_width_(0), back_buffer_height_(0) {
  set_screen_size(screen_width, screen_height);
  set_scale(screen_scale);

  // Get the actual device width/height (may be different than screen size)
  RECT rc;
  ::GetClientRect(hWnd, &rc);
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

  // Create DXGI factory to create the swap chain
  ComPtr<IDXGIFactory4> factory;
  UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
  dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
  HRESULT hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
  if (FAILED(hr))
    return;

  // Describe and create the swap chain
  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
  swapChainDesc.BufferCount = BufferCount;
  swapChainDesc.Width = width;
  swapChainDesc.Height = height;
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;

  ComPtr<IDXGISwapChain1> swapChain;
  hr = factory->CreateSwapChainForHwnd(
    context_->command_queue(), // D3D12 swap chain requires command queue
    hWnd,
    &swapChainDesc,
    nullptr,
    nullptr,
    &swapChain);

  if (FAILED(hr))
    return;

  // Disable Alt+Enter fullscreen toggle
  factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

  // Get IDXGISwapChain3 interface for GetCurrentBackBufferIndex()
  hr = swapChain.As(&swap_chain_);
  if (FAILED(hr))
    return;

  // Create RTVs for each back buffer
  for (UINT i = 0; i < BufferCount; i++) {
    hr = swap_chain_->GetBuffer(i, IID_PPV_ARGS(&render_targets_[i]));
    if (FAILED(hr))
      return;

    rtv_handles_[i] = context_->descriptor_allocator()->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    context_->device()->CreateRenderTargetView(render_targets_[i].Get(), &rtvDesc,
                                               rtv_handles_[i].cpu_handle());
  }

  render_buffer_id_ = driver_->NextRenderBufferId();

  back_buffer_width_ = width;
  back_buffer_height_ = height;
}

SwapChainD3D12::~SwapChainD3D12() {}

IDXGISwapChain3* SwapChainD3D12::swap_chain() { return swap_chain_.Get(); }

ID3D12Resource* SwapChainD3D12::render_target() {
  if (!swap_chain_)
    return nullptr;
  UINT index = swap_chain_->GetCurrentBackBufferIndex();
  return render_targets_[index].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChainD3D12::render_target_view() {
  UINT index = swap_chain_ ? swap_chain_->GetCurrentBackBufferIndex() : 0;
  return rtv_handles_[index].cpu_handle();
}

void SwapChainD3D12::PresentFrame() {
  if (swap_chain_)
    swap_chain_->Present(enable_vsync_ ? 1 : 0, 0);
}

void SwapChainD3D12::Resize(int width, int height) {
  set_screen_size(width, height);

  // Release back buffer references before resize
  for (UINT i = 0; i < BufferCount; i++)
    render_targets_[i].Reset();

  // Get the actual device width/height (may be different than screen size)
  RECT rc;
  ::GetClientRect(hwnd_, &rc);
  UINT client_width = rc.right - rc.left;
  UINT client_height = rc.bottom - rc.top;

  HRESULT hr = swap_chain_->ResizeBuffers(BufferCount, client_width, client_height,
                                          DXGI_FORMAT_UNKNOWN, 0);
  if (FAILED(hr)) {
    MessageBoxW(nullptr,
                L"SwapChainD3D12::Resize, unable to resize, IDXGISwapChain::ResizeBuffers failed.",
                L"Error", MB_OK);
    return;
  }

  // Recreate RTVs for each back buffer
  for (UINT i = 0; i < BufferCount; i++) {
    hr = swap_chain_->GetBuffer(i, IID_PPV_ARGS(&render_targets_[i]));
    if (FAILED(hr))
      return;

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    context_->device()->CreateRenderTargetView(render_targets_[i].Get(), &rtvDesc,
                                               rtv_handles_[i].cpu_handle());
  }

  back_buffer_width_ = client_width;
  back_buffer_height_ = client_height;
}

void SwapChainD3D12::set_scale(double scale) { scale_ = scale; }

double SwapChainD3D12::scale() const { return scale_; }

void SwapChainD3D12::set_screen_size(uint32_t width, uint32_t height) {
  width_ = width;
  height_ = height;
}

void SwapChainD3D12::screen_size(uint32_t& width, uint32_t& height) {
  width = width_;
  height = height_;
}

UINT SwapChainD3D12::back_buffer_width() { return back_buffer_width_; }

UINT SwapChainD3D12::back_buffer_height() { return back_buffer_height_; }

} // namespace ultralight

#endif
