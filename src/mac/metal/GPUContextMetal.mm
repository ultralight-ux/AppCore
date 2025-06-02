#import "GPUContextMetal.h"
#import "GPUDriverMetal.h"
#include "ShaderTypes.h"
#import <dispatch/dispatch.h>
#include <memory>
#include <functional>
#include <AppCore/App.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>
#include <Ultralight/platform/FileSystem.h>
#import "../WindowMac.h"

namespace ultralight {
    
// Public domain from https://stackoverflow.com/a/27952689
inline size_t hash_combine( size_t lhs, size_t rhs ) {
#if defined(__x86_64__)
  lhs ^= rhs + 0x9e3779b97f4a7c15 + (lhs << 6) + (lhs >> 2);
#else
  lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
#endif
  return lhs;
}
    
RenderState::RenderState() : shader_type(ShaderType::Fill),
                             blend_enabled(true),
                             pixel_format(MTLPixelFormatBGRA8Unorm),
                             sample_count(1) {}

size_t RenderState::Hash() {
  size_t result = std::hash<uint32_t>{}((uint32_t)shader_type);
  result = hash_combine(result, std::hash<bool>{}(blend_enabled));
  result = hash_combine(result, std::hash<uint32_t>{}((uint32_t)pixel_format));
  result = hash_combine(result, std::hash<uint32_t>{}((uint32_t)sample_count));
  return result;
}

GPUContextMetal::GPUContextMetal(id<MTLDevice> device, bool enable_vsync, bool enable_msaa) {
  device_ = device;
  msaa_enabled_ = enable_msaa;
  msaa_enabled_ = false; // Disable MSAA for now, it's causing issues with Metal on macOS
  
  // Only allow FrameCount - 1 number of frames in-flight-- testing on MacBook Pros older than 2016 shows corruption
  // if we use semaphore == FrameCount. 
  fence_ = dispatch_semaphore_create(FrameCount - 1);
  
  // Initialize shader libraries - will be loaded on-demand in LoadShaders()
  library_ = nullptr;
  
  // Create the command queue
  command_queue_ = [device_ newCommandQueue];
  
  MTLCaptureManager *sharedCaptureManager = [MTLCaptureManager sharedCaptureManager];
  id<MTLCaptureScope> myCaptureScope = [sharedCaptureManager newCaptureScopeWithDevice:device_];
  myCaptureScope.label = @"Draw Ultralight Frame";
  sharedCaptureManager.defaultCaptureScope = myCaptureScope;
  
  driver_.reset(new ultralight::GPUDriverMetal(this));
}
    
GPUContextMetal::~GPUContextMetal() {}

void GPUContextMetal::BeginDrawing() {
  // We may have uncommitted command buffers from UpdateTextureResource calls when synchronizing textures
  // Commit them now before creating new command buffer below.
  CommitCommandBuffer();
  
  // Wait to ensure only `FrameCount` number of frames are getting processed
  // by any stage in the Metal pipeline (CPU, GPU, Metal, Drivers, etc.).
  dispatch_semaphore_wait(fence_, DISPATCH_TIME_FOREVER);
  
  frame_index_ = (frame_index_ + 1) % FrameCount;
  
  MTLCaptureManager *sharedCaptureManager = [MTLCaptureManager sharedCaptureManager];
  
  [sharedCaptureManager.defaultCaptureScope beginScope];
  
  // Create a new command buffer for each render pass to the current drawable
  command_buffer_ = [command_queue_ commandBuffer];
  command_buffer_.label = @"UltralightCommandBuffer";
}
    
void GPUContextMetal::EndDrawing() {
  if (!command_buffer_)
    return;
  
  driver_->EndDrawing();
  
  // Add a completion handler that signals `fence_` when Metal and the GPU have fully
  // finished processing the commands that were encoded for this frame.
  // This completion indicates that the dynamic buffers that were written-to in this frame, are no
  // longer needed by Metal and the GPU; therefore, the CPU can overwrite the buffer contents
  // without corrupting any rendering operations.
  __block dispatch_semaphore_t block_semaphore = fence_;
  [command_buffer_ addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
    dispatch_semaphore_signal(block_semaphore);
  }];
  
  CommitCommandBuffer();
  
  MTLCaptureManager *sharedCaptureManager = [MTLCaptureManager sharedCaptureManager];
  [sharedCaptureManager.defaultCaptureScope endScope];
}

void GPUContextMetal::AddWindow(WindowMac* window) {
  windows_by_render_buffer_id_[window->render_buffer_id()] = window;
}

void GPUContextMetal::RemoveWindow(WindowMac* window) {
  auto i = windows_by_render_buffer_id_.find(window->render_buffer_id());
  if (i != windows_by_render_buffer_id_.end())
    windows_by_render_buffer_id_.erase(i);
}

WindowMac* GPUContextMetal::GetWindowByRenderBufferId(uint32_t render_buffer_id) {
  auto i = windows_by_render_buffer_id_.find(render_buffer_id);
  if (i != windows_by_render_buffer_id_.end())
    return i->second;

  return nullptr;
}

void GPUContextMetal::LoadShaders() {
  if (!shader_libraries_.empty())
    return;

  // Load FillPath shader (uses vertex_path and fill_path)
  NSData* vertex_path_data = [NSData dataWithBytes:vertex_path_vs_data length:vertex_path_vs_size];
  NSData* fill_path_data = [NSData dataWithBytes:fill_path_ps_data length:fill_path_ps_size];
  id<MTLLibrary> vertex_path_lib = [device_ newLibraryWithData:vertex_path_data error:nil];
  id<MTLLibrary> fill_path_lib = [device_ newLibraryWithData:fill_path_data error:nil];
  shader_libraries_[ShaderType::FillPath] = std::make_pair(vertex_path_lib, fill_path_lib);

  // Load Fill shader (uses vertex_quad and fill)
  NSData* vertex_quad_data = [NSData dataWithBytes:vertex_quad_vs_data length:vertex_quad_vs_size];
  NSData* fill_data = [NSData dataWithBytes:fill_ps_data length:fill_ps_size];
  id<MTLLibrary> vertex_quad_lib = [device_ newLibraryWithData:vertex_quad_data error:nil];
  id<MTLLibrary> fill_lib = [device_ newLibraryWithData:fill_data error:nil];
  shader_libraries_[ShaderType::Fill] = std::make_pair(vertex_quad_lib, fill_lib);

  // Load FilterBasic shader (uses vertex_quad and filter_basic)
  NSData* filter_basic_data = [NSData dataWithBytes:filter_basic_ps_data length:filter_basic_ps_size];
  id<MTLLibrary> filter_basic_lib = [device_ newLibraryWithData:filter_basic_data error:nil];
  shader_libraries_[ShaderType::FilterBasic] = std::make_pair(vertex_quad_lib, filter_basic_lib);

  // Load FilterBlur shader (uses vertex_quad and filter_blur)
  NSData* filter_blur_data = [NSData dataWithBytes:filter_blur_ps_data length:filter_blur_ps_size];
  id<MTLLibrary> filter_blur_lib = [device_ newLibraryWithData:filter_blur_data error:nil];
  shader_libraries_[ShaderType::FilterBlur] = std::make_pair(vertex_quad_lib, filter_blur_lib);

  // Load FilterDropShadow shader (uses vertex_quad and filter_dropshadow)
  NSData* filter_dropshadow_data = [NSData dataWithBytes:filter_dropshadow_ps_data length:filter_dropshadow_ps_size];
  id<MTLLibrary> filter_dropshadow_lib = [device_ newLibraryWithData:filter_dropshadow_data error:nil];
  shader_libraries_[ShaderType::FilterDropShadow] = std::make_pair(vertex_quad_lib, filter_dropshadow_lib);
}
    
id<MTLRenderPipelineState> GPUContextMetal::render_pipeline_state() {
  LoadShaders();
  
  uint32_t hash = render_state_.Hash();
  
  auto i = render_pipeline_states_.find(hash);
  if (i != render_pipeline_states_.end())
    return i->second;
  
  // RenderPipelineState hasn't yet been created for this RenderState permutation. Create it now.
  
  auto shader_iter = shader_libraries_.find(render_state_.shader_type);
  if (shader_iter == shader_libraries_.end()) {
    NSLog(@"Failed to create pipeline state, shader type not loaded: %d", (int)render_state_.shader_type);
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:@"Failed to create pipeline state, shader type not loaded"];
    [alert runModal];
    exit(-1);
  }
  
  id<MTLLibrary> vertex_library = shader_iter->second.first;
  id<MTLLibrary> fragment_library = shader_iter->second.second;
  
  MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
  pipelineStateDescriptor.label = @"Ultralight RenderPipelineState";
  pipelineStateDescriptor.vertexFunction = [vertex_library newFunctionWithName:@"main"];
  pipelineStateDescriptor.fragmentFunction = [fragment_library newFunctionWithName:@"main"];
  
  pipelineStateDescriptor.sampleCount = render_state_.sample_count;
  pipelineStateDescriptor.colorAttachments[0].pixelFormat = render_state_.pixel_format;
  
  auto colorAttachmentDesc = pipelineStateDescriptor.colorAttachments[0];
  if (render_state_.blend_enabled) {
    colorAttachmentDesc.blendingEnabled = YES;
    colorAttachmentDesc.rgbBlendOperation = MTLBlendOperationAdd;
    colorAttachmentDesc.alphaBlendOperation = MTLBlendOperationAdd;
    colorAttachmentDesc.sourceRGBBlendFactor = MTLBlendFactorOne;
    colorAttachmentDesc.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    colorAttachmentDesc.sourceAlphaBlendFactor = MTLBlendFactorOneMinusDestinationAlpha;
    colorAttachmentDesc.destinationAlphaBlendFactor = MTLBlendFactorOne;
    colorAttachmentDesc.writeMask = MTLColorWriteMaskAll;
  } else {
    colorAttachmentDesc.blendingEnabled = NO;
    colorAttachmentDesc.rgbBlendOperation = MTLBlendOperationAdd;
    colorAttachmentDesc.alphaBlendOperation = MTLBlendOperationAdd;
    colorAttachmentDesc.sourceRGBBlendFactor = MTLBlendFactorOne;
    colorAttachmentDesc.destinationRGBBlendFactor = MTLBlendFactorZero;
    colorAttachmentDesc.sourceAlphaBlendFactor = MTLBlendFactorOne;
    colorAttachmentDesc.destinationAlphaBlendFactor = MTLBlendFactorZero;
    colorAttachmentDesc.writeMask = MTLColorWriteMaskAll;
  }
  
  NSError *error = NULL;
  id<MTLRenderPipelineState> result = [device_ newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                                              error:&error];
  if (!result)
  {
    // Pipeline State creation could fail if we haven't properly set up our pipeline descriptor.
    //  If the Metal API validation is enabled, we can find out more information about what
    //  went wrong.  (Metal API validation is enabled by default when a debug build is run
    //  from Xcode)
    NSLog(@"Failed to create pipeline state, error %@", error);
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:@"Failed to create pipeline state."];
    [alert runModal];
    exit(-1);
  }
  
  render_pipeline_states_[hash] = result;
  return result;
}
    
id<MTLCommandBuffer> GPUContextMetal::command_buffer() {
  if (!command_buffer_) {
    command_buffer_ = [command_queue_ commandBuffer];
    command_buffer_.label = @"UltralightCommandBuffer";
  }
  
  return command_buffer_;
}

void GPUContextMetal::CommitCommandBuffer() {
  if (!command_buffer_)
    return;
  
  [command_buffer_ commit];
  [command_buffer_ waitUntilCompleted];
  command_buffer_ = nullptr;
}

}  // namespace ultralight
