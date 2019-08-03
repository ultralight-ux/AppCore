#import "GPUContextMetal.h"
#import "GPUDriverMetal.h"
#include "../../../shaders/metal/bin/metal_shader.h"
#import <dispatch/dispatch.h>
#include <memory>
#include <AppCore/App.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>
#include <Ultralight/platform/FileSystem.h>

namespace ultralight {

GPUContextMetal::GPUContextMetal(MTKView* view, int screen_width, int screen_height, double screen_scale, bool fullscreen, bool enable_vsync) {
    device_ = view.device;
    view_ = view;
    
    set_screen_size(screen_width, screen_height);
    set_scale(screen_scale);
    
    NSError *error = NULL;
    
    id<MTLLibrary> library;
    
    // Load all the shader files with a .metal file extension in the project
    //id<MTLLibrary> defaultLibrary = [device_ newDefaultLibrary];
    
    if (App::instance()->settings().load_shaders_from_file_system) {
        auto fs = ultralight::Platform::instance().file_system();
        if (!fs) {
            NSAlert *alert = [[NSAlert alloc] init];
            [alert setMessageText:@"Could not load shaders, null FileSystem instance."];
            [alert runModal];
            exit(-1);
        }
        ultralight::FileHandle handle = fs->OpenFile("shaders/metal/src/Shaders.metal", false);
        
        if (handle == ultralight::invalidFileHandle) {
            NSAlert *alert = [[NSAlert alloc] init];
            [alert setMessageText:@"Could not load shaders, 'shaders/metal/src/Shaders.metal' not found in FileSystem."];
            [alert runModal];
            exit(-1);
        }
        
        int64_t file_size = 0;
        fs->GetFileSize(handle, file_size);
        std::unique_ptr<char[]> buffer(new char[file_size]);
        fs->ReadFromFile(handle, buffer.get(), file_size);
        fs->CloseFile(handle);
        
        NSError* compileError;
        library = [device_ newLibraryWithSource:[NSString stringWithUTF8String:buffer.get()] options:nil error:&compileError];
        if (!library) {
            NSLog(@"Could not compile shader: %@", compileError);
            NSAlert *alert = [[NSAlert alloc] init];
            [alert setMessageText:@"Could not compile shader."];
            [alert runModal];
            exit(-1);
        }
    } else {
        dispatch_data_t data = dispatch_data_create(metal_shader, metal_shader_len, NULL, DISPATCH_DATA_DESTRUCTOR_DEFAULT);
        library = [device_ newLibraryWithData:data error:nil];
    }
    
    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineStateDescriptor.label = @"Fill Pipeline";
    pipelineStateDescriptor.vertexFunction = [library newFunctionWithName:@"vertexShader"];
    pipelineStateDescriptor.fragmentFunction = [library newFunctionWithName:@"fragmentShader"];
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat;
    
    auto colorAttachmentDesc = pipelineStateDescriptor.colorAttachments[0];
    colorAttachmentDesc.blendingEnabled = true;
    colorAttachmentDesc.rgbBlendOperation = MTLBlendOperationAdd;
    colorAttachmentDesc.alphaBlendOperation = MTLBlendOperationAdd;
    colorAttachmentDesc.sourceRGBBlendFactor = MTLBlendFactorOne;
    colorAttachmentDesc.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    colorAttachmentDesc.sourceAlphaBlendFactor = MTLBlendFactorOneMinusDestinationAlpha;
    colorAttachmentDesc.destinationAlphaBlendFactor = MTLBlendFactorOne;
    
    render_pipeline_state_ = [device_ newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                                     error:&error];
    if (!render_pipeline_state_)
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
    
    MTLRenderPipelineDescriptor *pipelineStateNoBlendDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineStateNoBlendDescriptor.label = @"Fill Pipeline No Blend";
    pipelineStateNoBlendDescriptor.vertexFunction = [library newFunctionWithName:@"vertexShader"];
    pipelineStateNoBlendDescriptor.fragmentFunction = [library newFunctionWithName:@"fragmentShader"];
    pipelineStateNoBlendDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat;
    
    auto colorAttachmentNoBlendDesc = pipelineStateNoBlendDescriptor.colorAttachments[0];
    colorAttachmentNoBlendDesc.blendingEnabled = false;

    render_pipeline_state_no_blend_ = [device_ newRenderPipelineStateWithDescriptor:pipelineStateNoBlendDescriptor
                                                                              error:&error];
    if (!render_pipeline_state_no_blend_)
    {
        // Pipeline State creation could fail if we haven't properly set up our pipeline descriptor.
        //  If the Metal API validation is enabled, we can find out more information about what
        //  went wrong.  (Metal API validation is enabled by default when a debug build is run
        //  from Xcode)
        NSLog(@"Failed to create pipeline state (no blend), error %@", error);
        NSAlert *alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Failed to create pipeline state (no blend)."];
        [alert runModal];
        exit(-1);
    }
    
    MTLRenderPipelineDescriptor *pathPipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pathPipelineStateDescriptor.label = @"Fill Path Pipeline";
    pathPipelineStateDescriptor.vertexFunction = [library newFunctionWithName:@"pathVertexShader"];
    pathPipelineStateDescriptor.fragmentFunction = [library newFunctionWithName:@"pathFragmentShader"];
    pathPipelineStateDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat;
    
    auto pathColorAttachmentDesc = pathPipelineStateDescriptor.colorAttachments[0];
    pathColorAttachmentDesc.blendingEnabled = true;
    pathColorAttachmentDesc.rgbBlendOperation = MTLBlendOperationAdd;
    pathColorAttachmentDesc.alphaBlendOperation = MTLBlendOperationAdd;
    pathColorAttachmentDesc.sourceRGBBlendFactor = MTLBlendFactorOne;
    pathColorAttachmentDesc.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    pathColorAttachmentDesc.sourceAlphaBlendFactor = MTLBlendFactorOneMinusDestinationAlpha;
    pathColorAttachmentDesc.destinationAlphaBlendFactor = MTLBlendFactorOne;
    
    path_render_pipeline_state_ = [device_ newRenderPipelineStateWithDescriptor:pathPipelineStateDescriptor
                                                                          error:&error];
    if (!path_render_pipeline_state_)
    {
        // Pipeline State creation could fail if we haven't properly set up our pipeline descriptor.
        //  If the Metal API validation is enabled, we can find out more information about what
        //  went wrong.  (Metal API validation is enabled by default when a debug build is run
        //  from Xcode)
        NSLog(@"Failed to create path pipeline state, error %@", error);
        NSAlert *alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Failed to create path pipeline state."];
        [alert runModal];
        exit(-1);
    }
    
    MTLRenderPipelineDescriptor *pathPipelineStateNoBlendDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pathPipelineStateNoBlendDescriptor.label = @"Fill Path Pipeline No Blend";
    pathPipelineStateNoBlendDescriptor.vertexFunction = [library newFunctionWithName:@"pathVertexShader"];
    pathPipelineStateNoBlendDescriptor.fragmentFunction = [library newFunctionWithName:@"pathFragmentShader"];
    pathPipelineStateNoBlendDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat;
    
    auto pathColorAttachmentNoBlendDesc = pathPipelineStateNoBlendDescriptor.colorAttachments[0];
    pathColorAttachmentNoBlendDesc.blendingEnabled = false;

    path_render_pipeline_state_no_blend_ = [device_ newRenderPipelineStateWithDescriptor:pathPipelineStateNoBlendDescriptor
                                                                                   error:&error];
    if (!path_render_pipeline_state_no_blend_)
    {
        // Pipeline State creation could fail if we haven't properly set up our pipeline descriptor.
        //  If the Metal API validation is enabled, we can find out more information about what
        //  went wrong.  (Metal API validation is enabled by default when a debug build is run
        //  from Xcode)
        NSLog(@"Failed to create path pipeline state (no blend), error %@", error);
        NSAlert *alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Failed to create path pipeline state (no blend)."];
        [alert runModal];
        exit(-1);
    }
    
    // Create the command queue
    command_queue_ = [device_ newCommandQueue];
    
    MTLCaptureManager *sharedCaptureManager = [MTLCaptureManager sharedCaptureManager];
    id<MTLCaptureScope> myCaptureScope = [sharedCaptureManager newCaptureScopeWithDevice:device_];
    myCaptureScope.label = @"Draw Ultralight Frame";
    sharedCaptureManager.defaultCaptureScope = myCaptureScope;
    
    driver_.reset(new ultralight::GPUDriverMetal(this));
}
    
GPUContextMetal::~GPUContextMetal() {
}

void GPUContextMetal::BeginDrawing() {
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
    
    [command_buffer_ commit];
    [command_buffer_ waitUntilScheduled];
    command_buffer_ = nullptr;
    
    MTLCaptureManager *sharedCaptureManager = [MTLCaptureManager sharedCaptureManager];
    [sharedCaptureManager.defaultCaptureScope endScope];
}
    
void GPUContextMetal::PresentFrame() {
    if (command_buffer_ && view_.currentDrawable)
        [command_buffer_ presentDrawable:view_.currentDrawable];
}
    
void GPUContextMetal::Resize(int width, int height) {
    set_screen_size(width, height);
}
    
}  // namespace ultralight
