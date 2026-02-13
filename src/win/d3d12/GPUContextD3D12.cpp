#if defined(DRIVER_D3D12)
#include "GPUContextD3D12.h"
#include "SwapChainD3D12.h"
#include "d3dx12.h"
#include <cassert>
#include <string>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <Ultralight/platform/GPUDriver.h>
// Include generated shader headers (D3D12 uses same FXC SM 4.0 bytecode as D3D11)
#include "d3d12/shaders.h"

namespace {

// Uniforms struct - must match GPUDriverD3D12's copy
struct Uniforms {
  DirectX::XMFLOAT4 State;
  DirectX::XMMATRIX Transform;
  DirectX::XMINT4 Integer4[2];
  DirectX::XMFLOAT4 Scalar4[2];
  DirectX::XMFLOAT4 Vector[8];
  DirectX::XMINT4 ClipData;
  DirectX::XMMATRIX Clip[8];
};

// D3D12 constant buffers must be 256-byte aligned
static constexpr UINT kUniformsAlignedSize = (sizeof(Uniforms) + 255) & ~255;

}  // namespace

namespace ultralight {

// Map platform-agnostic BlendFactor enum to D3D12 blend constants
static D3D12_BLEND MapBlendFactor(uint8_t factor) {
  switch (static_cast<BlendFactor>(factor)) {
  case BlendFactor::Zero:             return D3D12_BLEND_ZERO;
  case BlendFactor::One:              return D3D12_BLEND_ONE;
  case BlendFactor::SrcColor:         return D3D12_BLEND_SRC_COLOR;
  case BlendFactor::InvSrcColor:      return D3D12_BLEND_INV_SRC_COLOR;
  case BlendFactor::SrcAlpha:         return D3D12_BLEND_SRC_ALPHA;
  case BlendFactor::InvSrcAlpha:      return D3D12_BLEND_INV_SRC_ALPHA;
  case BlendFactor::DestColor:        return D3D12_BLEND_DEST_COLOR;
  case BlendFactor::InvDestColor:     return D3D12_BLEND_INV_DEST_COLOR;
  case BlendFactor::DestAlpha:        return D3D12_BLEND_DEST_ALPHA;
  case BlendFactor::InvDestAlpha:     return D3D12_BLEND_INV_DEST_ALPHA;
  case BlendFactor::SrcAlphaSaturate: return D3D12_BLEND_SRC_ALPHA_SAT;
  default:                            return D3D12_BLEND_ONE;
  }
}

// Map platform-agnostic BlendEquation enum to D3D12 blend operation
static D3D12_BLEND_OP MapBlendEquation(uint8_t equation) {
  switch (static_cast<BlendEquation>(equation)) {
  case BlendEquation::Add:         return D3D12_BLEND_OP_ADD;
  case BlendEquation::Subtract:    return D3D12_BLEND_OP_SUBTRACT;
  case BlendEquation::RevSubtract: return D3D12_BLEND_OP_REV_SUBTRACT;
  case BlendEquation::Min:         return D3D12_BLEND_OP_MIN;
  case BlendEquation::Max:         return D3D12_BLEND_OP_MAX;
  default:                         return D3D12_BLEND_OP_ADD;
  }
}

// Public domain from https://stackoverflow.com/a/27952689
inline size_t hash_combine(size_t lhs, size_t rhs) {
  lhs ^= rhs + 0x9e3779b97f4a7c15 + (lhs << 6) + (lhs >> 2);
  return lhs;
}

RenderState::RenderState() {
  Reset();
}

size_t RenderState::Hash() {
  size_t result = std::hash<uint32_t>{}((uint32_t)shader_type);
  result = hash_combine(result, std::hash<bool>{}(blend_enabled));
  result = hash_combine(result, std::hash<uint8_t>{}(blend_src_factor));
  result = hash_combine(result, std::hash<uint8_t>{}(blend_dst_factor));
  result = hash_combine(result, std::hash<uint8_t>{}(blend_equation));
  result = hash_combine(result, std::hash<uint32_t>{}((uint32_t)render_target_format));
  result = hash_combine(result, std::hash<UINT>{}(sample_count));
  return result;
}

void RenderState::Reset() {
  shader_type = ShaderType::Fill;
  blend_enabled = true;
  blend_src_factor = (uint8_t)BlendFactor::One;
  blend_dst_factor = (uint8_t)BlendFactor::InvSrcAlpha;
  blend_equation = (uint8_t)BlendEquation::Add;
  render_target_format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sample_count = 1;
}

GPUContextD3D12::GPUContextD3D12() {}

GPUContextD3D12::~GPUContextD3D12() {
  WaitForGpu();

  for (UINT frame_id = 0; frame_id < FrameCount; frame_id++) {
    auto& f = frames_[frame_id];
    f.resources_to_release_.clear();
    f.handles_to_release_.clear();
    f.constant_buffer_pool_.pool_.clear();
  }
}

bool GPUContextD3D12::Initialize() {
  HRESULT hr = S_OK;

  UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
  {
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
      debugController->EnableDebugLayer();
      dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
  }
#endif

  ComPtr<IDXGIFactory4> factory;
  hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
  if (FAILED(hr))
    return false;

  ComPtr<IDXGIAdapter1> hardwareAdapter;
  GetHardwareAdapter(factory.Get(), &hardwareAdapter);

  hr = D3D12CreateDevice(
    hardwareAdapter.Get(),
    D3D_FEATURE_LEVEL_11_0,
    IID_PPV_ARGS(&device_)
  );
  if (FAILED(hr))
    return false;

  // Create D3D12MA allocator (v3.0.1 requires pAdapter)
  D3D12MA::ALLOCATOR_DESC alloc_desc = {};
  alloc_desc.Flags = D3D12MA::ALLOCATOR_FLAG_NONE;
  alloc_desc.pDevice = device_.Get();
  alloc_desc.PreferredBlockSize = 32ull * 1024 * 1024;

  ComPtr<IDXGIAdapter> adapter;
  hardwareAdapter.As(&adapter);
  alloc_desc.pAdapter = adapter.Get();

  D3D12MA::Allocator* rawAllocator = nullptr;
  hr = D3D12MA::CreateAllocator(&alloc_desc, &rawAllocator);
  if (FAILED(hr))
    return false;
  allocator_.Attach(rawAllocator);

  // Create descriptor allocator
  descriptor_allocator_.reset(new DescriptorAllocator(device_.Get()));

  // Create gpu descriptor ring buffer
  gpu_descriptor_ring_buffer_.reset(new GPUDescriptorRingBuffer(device_.Get()));

  // Create command queue
  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

  hr = device_->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&command_queue_));
  if (FAILED(hr))
    return false;

  // Create command allocators for each frame flight
  for (UINT n = 0; n < FrameCount; n++) {
    hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                         IID_PPV_ARGS(&frames_[n].command_allocator_));
    if (FAILED(hr))
      return false;
  }

  // Create root signature
  {
    D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
    featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

    if (FAILED(device_->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
      featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

    CD3DX12_DESCRIPTOR_RANGE1 ranges[2];
    ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);
    ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);

    CD3DX12_ROOT_PARAMETER1 rootParameters[1];
    rootParameters[0].InitAsDescriptorTable(_countof(ranges), &ranges[0], D3D12_SHADER_VISIBILITY_ALL);

    D3D12_STATIC_SAMPLER_DESC sampler = {};
    sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    sampler.MipLODBias = 0;
    sampler.MaxAnisotropy = 0;
    sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    sampler.MinLOD = 0.0f;
    sampler.MaxLOD = 0.0f;
    sampler.ShaderRegister = 0;
    sampler.RegisterSpace = 0;
    sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &sampler,
                               D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    hr = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion,
                                               &signature, &error);
    if (error)
      OutputDebugStringA((const char*)error->GetBufferPointer());
    if (FAILED(hr))
      return false;

    hr = device_->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
                                      IID_PPV_ARGS(&root_signature_));
    if (FAILED(hr))
      return false;
  }

  // Initialize frame index
  frame_index_ = 0;

  // Create command list (initially closed)
  hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
    frame().command_allocator_.Get(), nullptr, IID_PPV_ARGS(&command_list_));
  if (FAILED(hr))
    return false;

  command_list_->Close();

  LoadShaders();

  // Create fence and event
  {
    hr = device_->CreateFence(frame().fence_value_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
    if (FAILED(hr))
      return false;
    frame().fence_value_++;

    fence_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (fence_event_ == nullptr)
      return false;
  }

  ResetCommandList();

  return true;
}

inline void CreateBlobFromData(const void* data, size_t size, ID3DBlob** ppBlobOut) {
  D3DCreateBlob(size, ppBlobOut);
  memcpy((*ppBlobOut)->GetBufferPointer(), data, (*ppBlobOut)->GetBufferSize());
}

bool GPUContextD3D12::LoadShaders() {
  if (!shaders_.empty())
    return true;

  // Vertex layout for FillPath shader (2f position, 4ub color, 2f texcoord)
  static const D3D12_INPUT_ELEMENT_DESC layout_2f_4ub_2f[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
  };

  // FillPath shader (vertex_path + fill_path)
  auto& shader_fill_path = shaders_[ShaderType::FillPath];
  shader_fill_path.input_layout = { layout_2f_4ub_2f, _countof(layout_2f_4ub_2f) };
  CreateBlobFromData(vertex_path_vs_data, vertex_path_vs_size, shader_fill_path.vertex_shader.GetAddressOf());
  CreateBlobFromData(fill_path_ps_data, fill_path_ps_size, shader_fill_path.pixel_shader.GetAddressOf());

  // Vertex layout for Fill/Filter shaders (2f position, 4ub color, 2f tex0, 2f tex1, 7x4f data)
  static const D3D12_INPUT_ELEMENT_DESC layout_2f_4ub_2f_2f_28f[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,      0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",    1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",    2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",    3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",    4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",    5, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",    6, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",    7, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
  };

  // Fill shader (vertex_quad + fill)
  auto& shader_fill = shaders_[ShaderType::Fill];
  shader_fill.input_layout = { layout_2f_4ub_2f_2f_28f, _countof(layout_2f_4ub_2f_2f_28f) };
  CreateBlobFromData(vertex_quad_vs_data, vertex_quad_vs_size, shader_fill.vertex_shader.GetAddressOf());
  CreateBlobFromData(fill_ps_data, fill_ps_size, shader_fill.pixel_shader.GetAddressOf());

  // FilterBasic shader (vertex_quad + filter_basic)
  auto& shader_filter_basic = shaders_[ShaderType::FilterBasic];
  shader_filter_basic.input_layout = { layout_2f_4ub_2f_2f_28f, _countof(layout_2f_4ub_2f_2f_28f) };
  CreateBlobFromData(vertex_quad_vs_data, vertex_quad_vs_size, shader_filter_basic.vertex_shader.GetAddressOf());
  CreateBlobFromData(filter_basic_ps_data, filter_basic_ps_size, shader_filter_basic.pixel_shader.GetAddressOf());

  // FilterBlur shader (vertex_quad + filter_blur)
  auto& shader_filter_blur = shaders_[ShaderType::FilterBlur];
  shader_filter_blur.input_layout = { layout_2f_4ub_2f_2f_28f, _countof(layout_2f_4ub_2f_2f_28f) };
  CreateBlobFromData(vertex_quad_vs_data, vertex_quad_vs_size, shader_filter_blur.vertex_shader.GetAddressOf());
  CreateBlobFromData(filter_blur_ps_data, filter_blur_ps_size, shader_filter_blur.pixel_shader.GetAddressOf());

  // FilterDropShadow shader (vertex_quad + filter_dropshadow)
  auto& shader_filter_dropshadow = shaders_[ShaderType::FilterDropShadow];
  shader_filter_dropshadow.input_layout = { layout_2f_4ub_2f_2f_28f, _countof(layout_2f_4ub_2f_2f_28f) };
  CreateBlobFromData(vertex_quad_vs_data, vertex_quad_vs_size, shader_filter_dropshadow.vertex_shader.GetAddressOf());
  CreateBlobFromData(filter_dropshadow_ps_data, filter_dropshadow_ps_size, shader_filter_dropshadow.pixel_shader.GetAddressOf());

  return true;
}

ID3D12Device* GPUContextD3D12::device() { assert(device_.Get()); return device_.Get(); }

D3D12MA::Allocator* GPUContextD3D12::allocator() { assert(allocator_.Get()); return allocator_.Get(); }

void GPUContextD3D12::BeginDrawing() {
  // Command list was already reset by MoveToNextFrame() (or Initialize() on first frame).
  // Set the GPU descriptor heap for this frame.
  ID3D12DescriptorHeap* heaps[] = { gpu_descriptor_ring_buffer_->descriptor_heap() };
  command_list_->SetDescriptorHeaps(_countof(heaps), heaps);
  command_list_->SetGraphicsRootSignature(root_signature_.Get());
}

void GPUContextD3D12::EndDrawing() {
  // Close and execute the command list, then advance to next frame.
  Flush();
}

void GPUContextD3D12::Flush() {
  HRESULT hr = command_list_->Close();
  if (FAILED(hr))
    return;

  ID3D12CommandList* ppCommandLists[] = { command_list_.Get() };
  command_queue_->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

  MoveToNextFrame();
}

void GPUContextD3D12::EnableBlend() {
  render_state_.blend_enabled = true;
  render_state_.blend_src_factor = (uint8_t)BlendFactor::One;
  render_state_.blend_dst_factor = (uint8_t)BlendFactor::InvSrcAlpha;
  render_state_.blend_equation = (uint8_t)BlendEquation::Add;
}

void GPUContextD3D12::DisableBlend() {
  render_state_.blend_enabled = false;
}

void GPUContextD3D12::SetBlendState(uint8_t src_factor, uint8_t dst_factor, uint8_t equation) {
  render_state_.blend_enabled = true;
  render_state_.blend_src_factor = src_factor;
  render_state_.blend_dst_factor = dst_factor;
  render_state_.blend_equation = equation;
}

void GPUContextD3D12::ReleaseWhenFrameComplete(ComPtr<D3D12MA::Allocation>&& resource) {
  frame().resources_to_release_.emplace_back(std::move(resource));
}

void GPUContextD3D12::ReleaseWhenFrameComplete(DescriptorHandle&& handle) {
  frame().handles_to_release_.emplace_back(std::move(handle));
}

ComPtr<ID3D12PipelineState> GPUContextD3D12::pipeline_state() {
  size_t hash = render_state_.Hash();

  auto i = pipeline_states_.find(hash);
  if (i != pipeline_states_.end())
    return i->second;

  // Create new pipeline state based on current RenderState
  auto& shader = shaders_[render_state_.shader_type];

  D3D12_RENDER_TARGET_BLEND_DESC rt_blend_desc = {};
  rt_blend_desc.BlendEnable = render_state_.blend_enabled;
  if (render_state_.blend_enabled) {
    rt_blend_desc.SrcBlend = MapBlendFactor(render_state_.blend_src_factor);
    rt_blend_desc.DestBlend = MapBlendFactor(render_state_.blend_dst_factor);
    rt_blend_desc.BlendOp = MapBlendEquation(render_state_.blend_equation);
    // Use same blend for alpha channel (works well for premultiplied alpha)
    rt_blend_desc.SrcBlendAlpha = rt_blend_desc.SrcBlend;
    rt_blend_desc.DestBlendAlpha = rt_blend_desc.DestBlend;
    rt_blend_desc.BlendOpAlpha = rt_blend_desc.BlendOp;
  }
  rt_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
  rt_blend_desc.LogicOpEnable = false;

  D3D12_BLEND_DESC blend_desc = {};
  blend_desc.AlphaToCoverageEnable = false;
  blend_desc.IndependentBlendEnable = false;
  blend_desc.RenderTarget[0] = rt_blend_desc;

  D3D12_RASTERIZER_DESC rasterizer_desc = {};
  rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;
  rasterizer_desc.CullMode = D3D12_CULL_MODE_NONE;
  rasterizer_desc.FrontCounterClockwise = FALSE;
  rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
  rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
  rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
  rasterizer_desc.DepthClipEnable = false;
#if ENABLE_MSAA
  rasterizer_desc.MultisampleEnable = TRUE;
  rasterizer_desc.AntialiasedLineEnable = TRUE;
#else
  rasterizer_desc.MultisampleEnable = FALSE;
  rasterizer_desc.AntialiasedLineEnable = FALSE;
#endif
  rasterizer_desc.ForcedSampleCount = 0;
  rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

  D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
  psoDesc.InputLayout = shader.input_layout;
  psoDesc.pRootSignature = root_signature_.Get();
  psoDesc.VS = CD3DX12_SHADER_BYTECODE(shader.vertex_shader.Get());
  psoDesc.PS = CD3DX12_SHADER_BYTECODE(shader.pixel_shader.Get());
  psoDesc.RasterizerState = rasterizer_desc;
  psoDesc.BlendState = blend_desc;
  psoDesc.DepthStencilState.DepthEnable = FALSE;
  psoDesc.DepthStencilState.StencilEnable = FALSE;
  psoDesc.SampleMask = UINT_MAX;
  psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  psoDesc.NumRenderTargets = 1;
  psoDesc.RTVFormats[0] = render_state_.render_target_format;
  psoDesc.SampleDesc.Count = render_state_.sample_count;
  psoDesc.SampleDesc.Quality = render_state_.sample_count > 1 ? DXGI_STANDARD_MULTISAMPLE_QUALITY_PATTERN : 0;

  ComPtr<ID3D12PipelineState>& pso = pipeline_states_[hash];
  HRESULT hr = device()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso));
  if (FAILED(hr))
    return nullptr;
  return pso;
}

// Multi-swap-chain support

void GPUContextD3D12::AddSwapChain(SwapChainD3D12* swap_chain) {
  swap_chains_by_render_buffer_id_[swap_chain->render_buffer_id()] = swap_chain;
}

void GPUContextD3D12::RemoveSwapChain(SwapChainD3D12* swap_chain) {
  auto i = swap_chains_by_render_buffer_id_.find(swap_chain->render_buffer_id());
  if (i != swap_chains_by_render_buffer_id_.end())
    swap_chains_by_render_buffer_id_.erase(i);
}

SwapChainD3D12* GPUContextD3D12::GetSwapChainByRenderBufferId(uint32_t render_buffer_id) {
  auto i = swap_chains_by_render_buffer_id_.find(render_buffer_id);
  if (i != swap_chains_by_render_buffer_id_.end())
    return i->second;
  return nullptr;
}

// Hardware adapter enumeration

_Use_decl_annotations_
void GPUContextD3D12::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter) {
  ComPtr<IDXGIAdapter1> adapter;
  *ppAdapter = nullptr;

  for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex) {
    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);

    if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
      continue;

    if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
      break;
  }

  *ppAdapter = adapter.Detach();
}

// Frame management

void GPUContextD3D12::MoveToNextFrame() {
  // Schedule a Signal command in the queue
  Frame& last_frame = frame();
  const UINT64 currentFenceValue = last_frame.fence_value_;
  command_queue_->Signal(fence_.Get(), currentFenceValue);

  // Advance frame index (independent of swap chain buffer index)
  frame_index_ = (frame_index_ + 1) % FrameCount;

  Frame& cur_frame = frame();

  // If the next frame is not ready to be rendered yet, wait until it is ready
  if (fence_->GetCompletedValue() < cur_frame.fence_value_) {
    fence_->SetEventOnCompletion(cur_frame.fence_value_, fence_event_);
    WaitForSingleObjectEx(fence_event_, INFINITE, FALSE);
  }

  // Set the fence value for the next frame
  cur_frame.fence_value_ = currentFenceValue + 1;

  // Release previous allocations for this frame
  cur_frame.resources_to_release_.clear();
  cur_frame.handles_to_release_.clear();
  cur_frame.constant_buffer_pool_.Reset();

  // Reset command list for current frame
  ResetCommandList();
}

void GPUContextD3D12::WaitForGpu() {
  if (!fence_ || !command_queue_)
    return;

  command_queue_->Signal(fence_.Get(), frame().fence_value_);

  fence_->SetEventOnCompletion(frame().fence_value_, fence_event_);
  WaitForSingleObjectEx(fence_event_, INFINITE, FALSE);

  frame().fence_value_++;
}

void GPUContextD3D12::ResetCommandList() {
  frame().command_allocator_->Reset();

  render_state_.Reset();

  command_list_->Reset(frame().command_allocator_.Get(), pipeline_state().Get());

  command_list_->SetGraphicsRootSignature(root_signature_.Get());

  ID3D12DescriptorHeap* ppHeaps[] = { gpu_descriptor_ring_buffer()->descriptor_heap() };
  command_list_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

// Constant buffer pool

GPUContextD3D12::ConstantBuffer* GPUContextD3D12::ConstantBufferPool::GetBufferForWriting(GPUContextD3D12* ctx) {
  if (pool_.size() > next_idx_) {
    return &pool_[next_idx_++];
  }

  // No available entries in pool, create new
  ConstantBuffer cb;

  D3D12MA::ALLOCATION_DESC allocationDesc = {};
  allocationDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

  D3D12MA::Allocation* alloc = nullptr;
  auto cb_desc = CD3DX12_RESOURCE_DESC::Buffer(kUniformsAlignedSize);
  HRESULT hr = ctx->allocator()->CreateResource(
    &allocationDesc,
    &cb_desc,
    D3D12_RESOURCE_STATE_GENERIC_READ,
    NULL,
    &alloc,
    __uuidof(ID3D12Resource),
    nullptr);
  if (FAILED(hr))
    return nullptr;

  D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
  cbv_desc.BufferLocation = alloc->GetResource()->GetGPUVirtualAddress();
  cbv_desc.SizeInBytes = kUniformsAlignedSize;

  cb.cbv_handle_ = ctx->descriptor_allocator()->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
  ctx->device()->CreateConstantBufferView(&cbv_desc, cb.cbv_handle_.cpu_handle());

  // Take ownership of alloc (COM pointer with refcount 1)
  cb.buffer_.Attach(alloc);

  pool_.emplace_back(std::move(cb));
  next_idx_++;
  return &pool_.back();
}

}  // namespace ultralight

#endif
