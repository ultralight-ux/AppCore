#if defined(DRIVER_D3D12)
#include "GPUContextD3D12.h"
#include "d3dx12.h"
#include <cassert>
#include <string>
#include <directxcolors.h>
#include <d3dcompiler.h>
#include "../../../shaders/hlsl/bin/fill_fxc.h"
#include "../../../shaders/hlsl/bin/fill_path_fxc.h"
#include "../../../shaders/hlsl/bin/v2f_c4f_t2f_fxc.h"
#include "../../../shaders/hlsl/bin/v2f_c4f_t2f_t2f_d28f_fxc.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/FileSystem.h>
#include <AppCore/App.h>

namespace {

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

}  // namespace (unnamed)

namespace ultralight {

// Public domain from https://stackoverflow.com/a/27952689
inline size_t hash_combine(size_t lhs, size_t rhs) {
#if defined(__x86_64__)
  lhs ^= rhs + 0x9e3779b97f4a7c15 + (lhs << 6) + (lhs >> 2);
#else
  lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
#endif
  return lhs;
}

RenderState::RenderState()
{
  Reset();
}

size_t RenderState::Hash() {
  size_t result = std::hash<uint32_t>{}((uint32_t)shader_type);
  result = hash_combine(result, std::hash<bool>{}(blend_enabled));
  result = hash_combine(result, std::hash<uint32_t>{}((uint32_t)render_target_format));
  result = hash_combine(result, std::hash<UINT>{}(sample_count));
  return result;
}

void RenderState::Reset() {
  shader_type = kShaderType_Fill;
  blend_enabled = true;
  render_target_format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sample_count = 1;
}

GPUContextD3D12::GPUContextD3D12() {}

GPUContextD3D12::~GPUContextD3D12() {
  WaitForGpu();

  for (UINT frame_id = 0; frame_id < FrameCount; frame_id++) {
    auto& frame = frames_[frame_id];
    frame.resources_to_release_.clear();
    frame.handles_to_release_.clear();
    frame.constant_buffer_pool_.pool_.clear();
    frame.rtv_handle_ = DescriptorHandle();
  }

  if (allocator_) {
    allocator_->Release();
    allocator_ = nullptr;
  }
}

bool GPUContextD3D12::Initialize(HWND hWnd, int screen_width, int screen_height, double screen_scale, bool fullscreen, bool enable_vsync, bool sRGB, int samples) {
  samples_ = samples;
#if ENABLE_MSAA
  samples_ = 4;
#endif
  enable_vsync_ = enable_vsync;
  set_screen_size(screen_width, screen_height);
  set_scale(screen_scale);

  HRESULT hr = S_OK;

  hwnd_ = hWnd;

  // Get the actual device width/height (may be different than screen size)
  RECT rc;
  ::GetClientRect(hWnd, &rc);
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

  UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
  // Enable the debug layer (requires the Graphics Tools "optional feature").
  // NOTE: Enabling the debug layer after device creation will invalidate the active device.
  {
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
      debugController->EnableDebugLayer();

      // Enable additional debug layers.
      dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
  }
#endif

  ComPtr<IDXGIFactory4> factory;
  ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

  ComPtr<IDXGIAdapter1> hardwareAdapter;
  GetHardwareAdapter(factory.Get(), &hardwareAdapter);

  ThrowIfFailed(D3D12CreateDevice(
    hardwareAdapter.Get(),
    D3D_FEATURE_LEVEL_11_0,
    IID_PPV_ARGS(&device_)
  ));

  D3D12MA::ALLOCATOR_DESC alloc_desc = {};
  alloc_desc.Flags = D3D12MA::ALLOCATOR_FLAG_NONE;
  alloc_desc.pDevice = device_.Get();
  alloc_desc.PreferredBlockSize = 32ull * 1024 * 1024;

  // Create allocator
  ThrowIfFailed(D3D12MA::CreateAllocator(&alloc_desc, &allocator_));

  // Create descriptor allocator
  descriptor_allocator_.reset(new DescriptorAllocator(device_.Get()));

  // Create gpu descriptor ring buffer
  gpu_descriptor_ring_buffer_.reset(new GPUDescriptorRingBuffer(device_.Get()));

  // Describe and create the command queue.
  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

  ThrowIfFailed(device_->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&command_queue_)));

  // Describe and create the swap chain.
  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
  swapChainDesc.BufferCount = FrameCount;
  swapChainDesc.Width = width;
  swapChainDesc.Height = height;
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;

  ComPtr<IDXGISwapChain1> swapChain;
  ThrowIfFailed(factory->CreateSwapChainForHwnd(
    command_queue_.Get(), // Swap chain needs the queue so that it can force a flush on it.
    hwnd_,
    &swapChainDesc,
    nullptr,
    nullptr,
    &swapChain
  ));

  // We don't support fullscreen transitions.
  ThrowIfFailed(factory->MakeWindowAssociation(hwnd_, DXGI_MWA_NO_ALT_ENTER));

  ThrowIfFailed(swapChain.As(&swap_chain_));
  frame_index_ = swap_chain_->GetCurrentBackBufferIndex();

  // Create frame resources.
  {
    // Create a RTV and a command allocator for each frame.
    for (UINT n = 0; n < FrameCount; n++)
    {

      ThrowIfFailed(swap_chain_->GetBuffer(n, IID_PPV_ARGS(&frames_[n].render_target_)));

      // Allocate an RTV descriptor
      frames_[n].rtv_handle_ = descriptor_allocator()->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

      // Create RTV
      D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
      rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
      rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
      device_->CreateRenderTargetView(frames_[n].render_target_.Get(), &rtv_desc, frames_[n].rtv_handle_.cpu_handle());

      ThrowIfFailed(device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&frames_[n].command_allocator_)));
    }
  }

  // ---------------------------------------------

  // Create the root signature.
  {
    D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

    // This is the highest version the context supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
    featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

    if (FAILED(device_->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
    {
      featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
    }

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
    rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    HRESULT hr = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error);

    if (error)
      OutputDebugStringA((const char*)error->GetBufferPointer());


    ThrowIfFailed(device_->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&root_signature_)));
  }

  // Create the command list with empty pipeline state
  ThrowIfFailed(device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
    frame().command_allocator_.Get(), nullptr, IID_PPV_ARGS(&command_list_)));

  // Command lists are created in the recording state, but there is nothing
  // to record yet. The main loop expects it to be closed, so close it now.
  ThrowIfFailed(command_list_->Close());

  LoadShaders();

  // Create synchronization objects and wait until assets have been uploaded to the GPU.
  {
    ThrowIfFailed(device_->CreateFence(frame().fence_value_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)));
    frame().fence_value_++;

    // Create an event handle to use for frame synchronization.
    fence_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (fence_event_ == nullptr)
    {
      ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
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

  static const D3D12_INPUT_ELEMENT_DESC layout_2f_4ub_2f[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
  };

  auto& shader_fill_path = shaders_[kShaderType_FillPath];
  shader_fill_path.input_layout = { layout_2f_4ub_2f, _countof(layout_2f_4ub_2f) };
  if (App::instance()->settings().load_shaders_from_file_system) {
    ThrowIfFailed(CompileShaderFromFile("shaders/hlsl/vs/v2f_c4f_t2f.hlsl", "VS", "vs_4_0", shader_fill_path.vertex_shader.GetAddressOf()));
    ThrowIfFailed(CompileShaderFromFile("shaders/hlsl/ps/fill_path.hlsl", "PS", "ps_4_0", shader_fill_path.pixel_shader.GetAddressOf()));
  }
  else {
    CreateBlobFromData(v2f_c4f_t2f_fxc, v2f_c4f_t2f_fxc_len, shader_fill_path.vertex_shader.GetAddressOf());
    CreateBlobFromData(fill_path_fxc, fill_path_fxc_len, shader_fill_path.pixel_shader.GetAddressOf());
  }

  static const D3D12_INPUT_ELEMENT_DESC layout_2f_4ub_2f_2f_28f[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
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

  auto& shader_fill = shaders_[kShaderType_Fill];
  shader_fill.input_layout = { layout_2f_4ub_2f_2f_28f, _countof(layout_2f_4ub_2f_2f_28f) };
  if (App::instance()->settings().load_shaders_from_file_system) {
    ThrowIfFailed(CompileShaderFromFile("shaders/hlsl/vs/v2f_c4f_t2f_t2f_d28f.hlsl", "VS", "vs_4_0", shader_fill.vertex_shader.GetAddressOf()));
    ThrowIfFailed(CompileShaderFromFile("shaders/hlsl/ps/fill.hlsl", "VS", "vs_4_0", shader_fill.pixel_shader.GetAddressOf()));
  }
  else {
    CreateBlobFromData(v2f_c4f_t2f_t2f_d28f_fxc, v2f_c4f_t2f_t2f_d28f_fxc_len, shader_fill.vertex_shader.GetAddressOf());
    CreateBlobFromData(fill_fxc, fill_fxc_len, shader_fill.pixel_shader.GetAddressOf());
  }

  return true;
}

ID3D12Device* GPUContextD3D12::device() { assert(device_.Get()); return device_.Get(); }

D3D12MA::Allocator* GPUContextD3D12::allocator() { assert(allocator_); return allocator_; }

void GPUContextD3D12::BeginDrawing() {
  // Indicate that the back buffer will be used as a render target.
  command_list_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(frame().render_target_.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
}

void GPUContextD3D12::EndDrawing() {}

void GPUContextD3D12::PresentFrame() {
  // Indicate that the back buffer will now be used to present.
  command_list_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(frame().render_target_.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

  ThrowIfFailed(command_list_->Close());

  // Execute the command list.
  ID3D12CommandList* ppCommandLists[] = { command_list_.Get() };
  command_queue_->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

  // Present the frame.
  ThrowIfFailed(swap_chain_->Present(1, 0));

  MoveToNextFrame();
}

void GPUContextD3D12::Resize(int width, int height) {
  set_screen_size(width, height);
}

// Scale is calculated from monitor DPI, see Application::SetScale
void GPUContextD3D12::set_scale(double scale) { scale_ = scale; }
double GPUContextD3D12::scale() const { return scale_; }

// This is in units, not actual pixels.
void GPUContextD3D12::set_screen_size(uint32_t width, uint32_t height) { width_ = width; height_ = height; }
void GPUContextD3D12::screen_size(uint32_t& width, uint32_t& height) { width = width_; height = height_; }

UINT GPUContextD3D12::back_buffer_width() { return back_buffer_width_; }
UINT GPUContextD3D12::back_buffer_height() { return back_buffer_height_; }

void GPUContextD3D12::ReleaseWhenFrameComplete(D3D12MA::ResourcePtr&& resource) {
  frame().resources_to_release_.emplace_back(std::move(resource));
}

void GPUContextD3D12::ReleaseWhenFrameComplete(DescriptorHandle&& handle) {
  frame().handles_to_release_.emplace_back(std::move(handle));
}

ComPtr<ID3D12PipelineState> GPUContextD3D12::pipeline_state() {
  uint32_t hash = render_state_.Hash();

  auto i = pipeline_states_.find(hash);
  if (i != pipeline_states_.end())
    return i->second;

  // Create the pipeline state, which includes compiling and loading shaders.
  auto& shader = shaders_[render_state_.shader_type];

  D3D12_RENDER_TARGET_BLEND_DESC rt_blend_desc = {};
  rt_blend_desc.BlendEnable = render_state_.blend_enabled;
  rt_blend_desc.SrcBlend = D3D12_BLEND_ONE;
  rt_blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
  rt_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
  rt_blend_desc.SrcBlendAlpha = D3D12_BLEND_INV_DEST_ALPHA;
  rt_blend_desc.DestBlendAlpha = D3D12_BLEND_ONE;
  rt_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
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

  // Describe and create the graphics pipeline state object (PSO).
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

  ComPtr<ID3D12PipelineState>& pipeline_state = pipeline_states_[hash];
  ThrowIfFailed(device()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipeline_state)));
  return pipeline_state;
}

_Use_decl_annotations_
void GPUContextD3D12::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter) {
  ComPtr<IDXGIAdapter1> adapter;
  *ppAdapter = nullptr;

  for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
  {
    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);

    if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
    {
      // Don't select the Basic Render Driver adapter (WARP)
      continue;
    }

    // Check to see if the adapter supports Direct3D 12, but don't create the
    // actual device yet.
    if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
    {
      break;
    }
  }

  *ppAdapter = adapter.Detach();
}

void GPUContextD3D12::MoveToNextFrame() {
  // Schedule a Signal command in the queue.
  Frame& last_frame = frame();
  const UINT64 currentFenceValue = last_frame.fence_value_;
  ThrowIfFailed(command_queue_->Signal(fence_.Get(), currentFenceValue));

  // Update the frame index.
  frame_index_ = swap_chain_->GetCurrentBackBufferIndex();

  Frame& cur_frame = frame();

  // If the next frame is not ready to be rendered yet, wait until it is ready.
  if (fence_->GetCompletedValue() < cur_frame.fence_value_) {
    ThrowIfFailed(fence_->SetEventOnCompletion(cur_frame.fence_value_, fence_event_));
    WaitForSingleObjectEx(fence_event_, INFINITE, FALSE);
  }

  // Set the fence value for the next frame.
  cur_frame.fence_value_ = currentFenceValue + 1;

  // We are about to start queuing commands for frame_index_. Release any previous allocations now.
  cur_frame.resources_to_release_.clear();
  cur_frame.handles_to_release_.clear();

  // Reset constant buffer pool
  cur_frame.constant_buffer_pool_.Reset();

  // Reset command list for current frame
  ResetCommandList();
}

void GPUContextD3D12::WaitForGpu() {
  // Schedule a Signal command in the queue.
  ThrowIfFailed(command_queue_->Signal(fence_.Get(), frame().fence_value_));

  // Wait until the fence has been processed.
  ThrowIfFailed(fence_->SetEventOnCompletion(frame().fence_value_, fence_event_));
  WaitForSingleObjectEx(fence_event_, INFINITE, FALSE);

  // Increment the fence value for the current frame.
  frame().fence_value_++;
}

void GPUContextD3D12::ResetCommandList() {
  ThrowIfFailed(frame().command_allocator_->Reset());

  render_state_.Reset();

  ThrowIfFailed(command_list_->Reset(frame().command_allocator_.Get(), pipeline_state().Get()));

  command_list_->SetGraphicsRootSignature(root_signature_.Get());

  ID3D12DescriptorHeap* ppHeaps[] = { gpu_descriptor_ring_buffer()->descriptor_heap() };
  command_list_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

GPUContextD3D12::ConstantBuffer* GPUContextD3D12::ConstantBufferPool::GetBufferForWriting(GPUContextD3D12* ctx) {
  if (pool_.size() > next_idx_) {
    return &pool_[next_idx_++];
  }

  // No available entries in pool, need to create new
  ConstantBuffer cb;

  D3D12MA::ALLOCATION_DESC allocationDesc = {};
  allocationDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
  
  D3D12MA::Allocation* alloc = nullptr;
  ThrowIfFailed(ctx->allocator()->CreateResource(
    &allocationDesc,
    &CD3DX12_RESOURCE_DESC::Buffer(sizeof(Uniforms)),
    D3D12_RESOURCE_STATE_GENERIC_READ,
    NULL,
    &alloc,
    __uuidof(ID3D12Resource),
    nullptr));

  D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
  cbv_desc.BufferLocation = alloc->GetResource()->GetGPUVirtualAddress();
  cbv_desc.SizeInBytes = sizeof(Uniforms);

  cb.cbv_handle_ = ctx->descriptor_allocator()->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
  ctx->device()->CreateConstantBufferView(&cbv_desc, cb.cbv_handle_.cpu_handle());

  // Take ownership of alloc
  cb.buffer_.reset(alloc);

  pool_.emplace_back(std::move(cb));
  next_idx_++;
  return &pool_.back();
}

}  // namespace ultralight

#endif
