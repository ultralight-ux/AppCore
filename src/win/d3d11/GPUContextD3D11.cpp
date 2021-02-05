#if defined(DRIVER_D3D11)
#include "GPUContextD3D11.h"
#include "SwapChainD3D11.h"
#include <cassert>

namespace ultralight {

GPUContextD3D11::GPUContextD3D11() {
  HRESULT hr = S_OK;
  feature_level_ = D3D_FEATURE_LEVEL_11_0;

  UINT deviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
  deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_FEATURE_LEVEL featureLevels[] = {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };

  unsigned numFeatureLevels = ARRAYSIZE(featureLevels);

  hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, featureLevels,
                         numFeatureLevels, D3D11_SDK_VERSION, device_.GetAddressOf(),
                         &feature_level_, immediate_context_.GetAddressOf());

  if (FAILED(hr)) {
    // Failed to create D3D11 device, fallback to CPU renderer
    immediate_context_.Reset();
    device_.Reset();
  }

  // Create Enabled Blend State

  D3D11_RENDER_TARGET_BLEND_DESC rt_blend_desc;
  ZeroMemory(&rt_blend_desc, sizeof(rt_blend_desc));
  rt_blend_desc.BlendEnable = true;
  rt_blend_desc.SrcBlend = D3D11_BLEND_ONE;
  rt_blend_desc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  rt_blend_desc.BlendOp = D3D11_BLEND_OP_ADD;
  rt_blend_desc.SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
  rt_blend_desc.DestBlendAlpha = D3D11_BLEND_ONE;
  rt_blend_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
  rt_blend_desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

  D3D11_BLEND_DESC blend_desc;
  ZeroMemory(&blend_desc, sizeof(blend_desc));
  blend_desc.AlphaToCoverageEnable = false;
  blend_desc.IndependentBlendEnable = false;
  blend_desc.RenderTarget[0] = rt_blend_desc;

  device()->CreateBlendState(&blend_desc, blend_state_.GetAddressOf());

  // Create Disabled Blend State

  ZeroMemory(&rt_blend_desc, sizeof(rt_blend_desc));
  rt_blend_desc.BlendEnable = false;
  rt_blend_desc.SrcBlend = D3D11_BLEND_ONE;
  rt_blend_desc.DestBlend = D3D11_BLEND_ZERO;
  rt_blend_desc.BlendOp = D3D11_BLEND_OP_ADD;
  rt_blend_desc.SrcBlendAlpha = D3D11_BLEND_ONE;
  rt_blend_desc.DestBlendAlpha = D3D11_BLEND_ZERO;
  rt_blend_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
  rt_blend_desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

  ZeroMemory(&blend_desc, sizeof(blend_desc));
  blend_desc.AlphaToCoverageEnable = false;
  blend_desc.IndependentBlendEnable = false;
  blend_desc.RenderTarget[0] = rt_blend_desc;

  device()->CreateBlendState(&blend_desc, disabled_blend_state_.GetAddressOf());

  EnableBlend();

  D3D11_RASTERIZER_DESC rasterizer_desc;
  ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));
  rasterizer_desc.FillMode = D3D11_FILL_SOLID;
  rasterizer_desc.CullMode = D3D11_CULL_NONE;
  rasterizer_desc.FrontCounterClockwise = false;
  rasterizer_desc.DepthBias = 0;
  rasterizer_desc.SlopeScaledDepthBias = 0.0f;
  rasterizer_desc.DepthBiasClamp = 0.0f;
  rasterizer_desc.DepthClipEnable = false;
  rasterizer_desc.ScissorEnable = false;
#if ENABLE_MSAA
  rasterizer_desc.MultisampleEnable = true;
  rasterizer_desc.AntialiasedLineEnable = true;
#else
  rasterizer_desc.MultisampleEnable = false;
  rasterizer_desc.AntialiasedLineEnable = false;
#endif

  device()->CreateRasterizerState(&rasterizer_desc, rasterizer_state_.GetAddressOf());

  D3D11_RASTERIZER_DESC scissor_rasterizer_desc;
  ZeroMemory(&scissor_rasterizer_desc, sizeof(scissor_rasterizer_desc));
  scissor_rasterizer_desc.FillMode = D3D11_FILL_SOLID;
  scissor_rasterizer_desc.CullMode = D3D11_CULL_NONE;
  scissor_rasterizer_desc.FrontCounterClockwise = false;
  scissor_rasterizer_desc.DepthBias = 0;
  scissor_rasterizer_desc.SlopeScaledDepthBias = 0.0f;
  scissor_rasterizer_desc.DepthBiasClamp = 0.0f;
  scissor_rasterizer_desc.DepthClipEnable = false;
  scissor_rasterizer_desc.ScissorEnable = true;
#if ENABLE_MSAA
  scissor_rasterizer_desc.MultisampleEnable = true;
  scissor_rasterizer_desc.AntialiasedLineEnable = true;
#else
  scissor_rasterizer_desc.MultisampleEnable = false;
  scissor_rasterizer_desc.AntialiasedLineEnable = false;
#endif

  device()->CreateRasterizerState(&scissor_rasterizer_desc,
                                  scissored_rasterizer_state_.GetAddressOf());

  DisableScissor();
}

GPUContextD3D11::~GPUContextD3D11() {
  if (device_) {
    immediate_context_->ClearState();

#ifdef _DEBUG
    ID3D11Debug* debug;
    HRESULT result
        = device_.Get()->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug));

    if (SUCCEEDED(result)) {
      debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
      debug->Release();
    }
#endif
  }
}

void GPUContextD3D11::BeginDrawing() { }

void GPUContextD3D11::EndDrawing() { }

ID3D11Device* GPUContextD3D11::device() { return device_.Get(); }

ID3D11DeviceContext* GPUContextD3D11::immediate_context() { return immediate_context_.Get(); }

void GPUContextD3D11::EnableBlend() {
  immediate_context_->OMSetBlendState(blend_state_.Get(), NULL, 0xffffffff);
}

void GPUContextD3D11::DisableBlend() {
  immediate_context_->OMSetBlendState(disabled_blend_state_.Get(), NULL, 0xffffffff);
}

void GPUContextD3D11::EnableScissor() {
  immediate_context_->RSSetState(scissored_rasterizer_state_.Get());
}

void GPUContextD3D11::DisableScissor() { immediate_context_->RSSetState(rasterizer_state_.Get()); }

void GPUContextD3D11::AddSwapChain(SwapChainD3D11* swap_chain) {
  swap_chains_by_render_buffer_id_[swap_chain->render_buffer_id()] = swap_chain;
}

void GPUContextD3D11::RemoveSwapChain(SwapChainD3D11* swap_chain) {
  auto i = swap_chains_by_render_buffer_id_.find(swap_chain->render_buffer_id());
  if (i != swap_chains_by_render_buffer_id_.end())
    swap_chains_by_render_buffer_id_.erase(i);
}

SwapChainD3D11* GPUContextD3D11::GetSwapChainByRenderBufferId(uint32_t render_buffer_id) {
  auto i = swap_chains_by_render_buffer_id_.find(render_buffer_id);
  if (i != swap_chains_by_render_buffer_id_.end())
    return i->second;

  return nullptr;
}

} // namespace ultralight

#endif
