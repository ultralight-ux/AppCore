#if defined(DRIVER_D3D11)
#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <stdint.h>
#include <Ultralight/Geometry.h>
#include <unordered_map>
#include <Ultralight/private/tracy/Tracy.hpp>
#ifdef TRACY_PROFILE_PERFORMANCE
#include <Ultralight/private/tracy/TracyD3D11.hpp>
#endif

#define ENABLE_MSAA 1

namespace ultralight {

class SwapChainD3D11;

class GPUContextD3D11 {
 public:
  GPUContextD3D11();

  virtual ~GPUContextD3D11();

  virtual void BeginDrawing();

  virtual void EndDrawing();

  virtual ID3D11Device* device();

  virtual ID3D11DeviceContext* immediate_context();

  virtual void EnableBlend();
  virtual void DisableBlend();

  // New method to set specific blend state
  virtual void SetBlendState(uint8_t src_factor, uint8_t dst_factor, uint8_t equation);

  virtual void EnableScissor();
  virtual void DisableScissor();

  void AddSwapChain(SwapChainD3D11* swap_chain);
  void RemoveSwapChain(SwapChainD3D11* swap_chain);

  SwapChainD3D11* GetSwapChainByRenderBufferId(uint32_t render_buffer_id);

#ifdef TRACY_PROFILE_PERFORMANCE
  TracyD3D11Ctx GetTracyContext() { return tracy_ctx_; }
#endif

 private:
  // Helper method to get or create a blend state based on blend parameters
  ID3D11BlendState* GetOrCreateBlendState(uint8_t src_factor, uint8_t dst_factor, uint8_t equation);

  // Create a unique key for blend state cache from parameters
  uint32_t MakeBlendStateKey(uint8_t src_factor, uint8_t dst_factor, uint8_t equation) const {
    return (static_cast<uint32_t>(src_factor) << 16) |
           (static_cast<uint32_t>(dst_factor) << 8) |
           static_cast<uint32_t>(equation);
  }

  Microsoft::WRL::ComPtr<ID3D11Device> device_;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context_;
  Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state_;
  Microsoft::WRL::ComPtr<ID3D11BlendState> disabled_blend_state_;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state_;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> scissored_rasterizer_state_;
  D3D_FEATURE_LEVEL feature_level_;

  // Cache for dynamically created blend states
  std::unordered_map<uint32_t, Microsoft::WRL::ComPtr<ID3D11BlendState>> blend_state_cache_;

  // Current blend state (for tracking)
  Microsoft::WRL::ComPtr<ID3D11BlendState> current_blend_state_;

  std::unordered_map<uint32_t, SwapChainD3D11*> swap_chains_by_render_buffer_id_;
#ifdef TRACY_PROFILE_PERFORMANCE
  TracyD3D11Ctx tracy_ctx_ = nullptr;
#endif
};

// Helper macro for GPU zones that automatically gets the Tracy context
// Usage: ProfileGPUZone("GPU_DrawGeometry")
// Requires context_ to be a GPUContextD3D11* member variable
#ifdef TRACY_PROFILE_PERFORMANCE
#define ProfileGPUZone(name) \
  auto ___tracy_gpu_ctx = context_->GetTracyContext(); \
  TracyD3D11Zone(___tracy_gpu_ctx, name)
#else
#define ProfileGPUZone(name)
#endif

} // namespace ultralight

#endif
