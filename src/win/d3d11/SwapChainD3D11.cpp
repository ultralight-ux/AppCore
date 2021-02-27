#if defined(DRIVER_D3D11)
#include "SwapChainD3D11.h"
#include "GPUDriverD3D11.h"
#include <cassert>

namespace ultralight {

SwapChainD3D11::SwapChainD3D11(GPUContextD3D11* context, GPUDriverD3D11* driver, HWND hWnd,
                               int screen_width, int screen_height, double screen_scale,
                               bool fullscreen, bool enable_vsync, bool sRGB, int samples)
    : context_(context), driver_(driver), hwnd_(hWnd), enable_vsync_(enable_vsync),
      render_buffer_id_(0) {
  samples_ = samples;
#if ENABLE_MSAA
  samples_ = 4;
#endif
  enable_vsync_ = enable_vsync;
  set_screen_size(screen_width, screen_height);
  set_scale(screen_scale);

  HRESULT hr = S_OK;

  // Get the actual device width/height (may be different than screen size)
  RECT rc;
  ::GetClientRect(hWnd, &rc);
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

  DXGI_SWAP_CHAIN_DESC sd;
  ::ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = !fullscreen;
  sd.Flags = fullscreen ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;

  IDXGIDevice* dxgiDevice = nullptr;
  hr = context_->device()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

  IDXGIAdapter* dxgiAdapter = nullptr;
  hr = dxgiDevice->GetAdapter(&dxgiAdapter);

  IDXGIFactory* dxgiFactory = nullptr;
  dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

  hr = dxgiFactory->CreateSwapChain(context_->device(), &sd, swap_chain_.GetAddressOf());

  if (FAILED(hr)) {
    // Unable to create swap chain
    swap_chain_.Reset();
    return;
  }

  // Create a render target view
  ID3D11Texture2D* pBackBuffer = nullptr;
  hr = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  if (FAILED(hr)) {
    // Unable get back buffer
    swap_chain_.Reset();
    return;
  }

  hr = context_->device()->CreateRenderTargetView(pBackBuffer, nullptr,
                                                  back_buffer_view_.GetAddressOf());
  pBackBuffer->Release();
  if (FAILED(hr)) {
    // Unable create back buffer view
    swap_chain_.Reset();
    return;
  }

  render_buffer_id_ = driver_->NextRenderBufferId();

  context_->immediate_context()->OMSetRenderTargets(1, back_buffer_view_.GetAddressOf(), nullptr);

  // Setup the viewport
  D3D11_VIEWPORT vp;
  ZeroMemory(&vp, sizeof(vp));
  vp.Width = (float)width * (float)scale();
  vp.Height = (float)height * (float)scale();
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  context_->immediate_context()->RSSetViewports(1, &vp);

  back_buffer_width_ = width;
  back_buffer_height_ = height;

  // Initialize backbuffer with white so we don't get flash of black while loading views.
  float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  context_->immediate_context()->ClearRenderTargetView(render_target_view(), color);
}

SwapChainD3D11::~SwapChainD3D11() { }

IDXGISwapChain* SwapChainD3D11::swap_chain() { return swap_chain_.Get(); }

ID3D11RenderTargetView* SwapChainD3D11::render_target_view() { return back_buffer_view_.Get(); }

void SwapChainD3D11::PresentFrame() { swap_chain()->Present(enable_vsync_ ? 1 : 0, 0); }

void SwapChainD3D11::Resize(int width, int height) {
  
  set_screen_size(width, height);

  context_->immediate_context()->OMSetRenderTargets(0, 0, 0);
  ID3D11Texture2D* back_buffer = nullptr;
  swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
  back_buffer->Release();

  back_buffer_view_.Reset();

  // Get the actual device width/height (may be different than screen size)
  RECT rc;
  ::GetClientRect(hwnd_, &rc);
  UINT client_width = rc.right - rc.left;
  UINT client_height = rc.bottom - rc.top;

  HRESULT hr;
  hr = swap_chain_->ResizeBuffers(0, client_width, client_height, DXGI_FORMAT_UNKNOWN, 0);
  if (FAILED(hr)) {
    MessageBoxW(nullptr,
                L"SwapChainD3D11::Resize, unable to resize, IDXGISwapChain::ResizeBuffers failed.",
                L"Error", MB_OK);
    exit(-1);
  }

  // Create a render target view
  ID3D11Texture2D* pBackBuffer = nullptr;
  hr = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  if (FAILED(hr)) {
    MessageBoxW(nullptr, L"SwapChainD3D11::Resize, unable to get back buffer.", L"Error", MB_OK);
    exit(-1);
  }

  hr = context_->device()->CreateRenderTargetView(pBackBuffer, nullptr,
                                                  back_buffer_view_.GetAddressOf());
  pBackBuffer->Release();
  if (FAILED(hr)) {
    MessageBoxW(nullptr, L"SwapChainD3D11::Resize, unable to create new render target view.",
                L"Error", MB_OK);
    exit(-1);
  }

  context_->immediate_context()->OMSetRenderTargets(1, back_buffer_view_.GetAddressOf(), nullptr);

  // Setup the viewport
  D3D11_VIEWPORT vp;
  ZeroMemory(&vp, sizeof(vp));
  vp.Width = (float)client_width * (float)scale();
  vp.Height = (float)client_height * (float)scale();
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  context_->immediate_context()->RSSetViewports(1, &vp);
}

// Scale is calculated from monitor DPI, see Application::SetScale
void SwapChainD3D11::set_scale(double scale) { scale_ = scale; }

double SwapChainD3D11::scale() const { return scale_; }

// This is in units, not actual pixels.
void SwapChainD3D11::set_screen_size(uint32_t width, uint32_t height) {
  width_ = width;
  height_ = height;
}

void SwapChainD3D11::screen_size(uint32_t& width, uint32_t& height) {
  width = width_;
  height = height_;
}

UINT SwapChainD3D11::back_buffer_width() { return back_buffer_width_; }

UINT SwapChainD3D11::back_buffer_height() { return back_buffer_height_; }

} // namespace ultralight

#endif