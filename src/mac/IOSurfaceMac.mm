#include "IOSurfaceMac.h"
#include <Ultralight/platform/Platform.h>
#include <iostream>
#include <CoreVideo/CoreVideo.h>

IOSurfaceMac::IOSurfaceMac(uint32_t width, uint32_t height)
    : width_(width)
    , height_(height)
    , row_bytes_(0)
    , surface_(nullptr)
    , pixels_(nullptr)
    , is_locked_(false) {
  CreateIOSurface(width, height);
}

IOSurfaceMac::~IOSurfaceMac() {
  DestroyIOSurface();
}

void IOSurfaceMac::CreateIOSurface(uint32_t width, uint32_t height) {
  // Clean up any existing IOSurface
  DestroyIOSurface();
  
  // Create IOSurface with properties for BGRA format
  CFMutableDictionaryRef properties = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                       &kCFTypeDictionaryKeyCallBacks,
                                       &kCFTypeDictionaryValueCallBacks);
  
  int32_t w = static_cast<int32_t>(width);
  int32_t h = static_cast<int32_t>(height);
  int32_t bytesPerElem = 4;
  int32_t allocSize = width * height * 4;
  int32_t pixelFormat = kCVPixelFormatType_32BGRA;
  
  CFNumberRef widthRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &w);
  CFNumberRef heightRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &h);
  CFNumberRef bytesPerElemRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &bytesPerElem);
  CFNumberRef allocSizeRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &allocSize);
  CFNumberRef pixelFormatRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &pixelFormat);
  
  CFDictionarySetValue(properties, kIOSurfaceWidth, widthRef);
  CFDictionarySetValue(properties, kIOSurfaceHeight, heightRef);
  CFDictionarySetValue(properties, kIOSurfaceBytesPerElement, bytesPerElemRef);
  CFDictionarySetValue(properties, kIOSurfacePixelFormat, pixelFormatRef);
  CFDictionarySetValue(properties, kIOSurfaceAllocSize, allocSizeRef);
  
  CFRelease(widthRef);
  CFRelease(heightRef);
  CFRelease(bytesPerElemRef);
  CFRelease(allocSizeRef);
  CFRelease(pixelFormatRef);
  
  surface_ = IOSurfaceCreate(properties);
  CFRelease(properties);
  if (surface_) {
    width_ = width;
    height_ = height;
    row_bytes_ = IOSurfaceGetBytesPerRow(surface_);
  } else {
    std::cerr << "Failed to create IOSurface with dimensions " << width << "x" << height << std::endl;
  }
}

void IOSurfaceMac::DestroyIOSurface() {
  if (is_locked_) {
    UnlockPixels();
  }
  
  if (surface_) {
    CFRelease(surface_);
    surface_ = nullptr;
  }
  
  pixels_ = nullptr;
}

uint32_t IOSurfaceMac::width() const {
  return width_;
}

uint32_t IOSurfaceMac::height() const {
  return height_;
}

uint32_t IOSurfaceMac::row_bytes() const {
  return row_bytes_;
}

size_t IOSurfaceMac::size() const {
  return row_bytes_ * height_;
}

void* IOSurfaceMac::LockPixels() {
  if (!surface_)
    return nullptr;
    
  // Lock the IOSurface for writing
  IOSurfaceLock(surface_, kIOSurfaceLockAvoidSync, nullptr);
  pixels_ = IOSurfaceGetBaseAddress(surface_);
  is_locked_ = true;
  
  return pixels_;
}

void IOSurfaceMac::UnlockPixels() {
  if (!surface_ || !is_locked_)
    return;
    
  // Unlock the IOSurface
  IOSurfaceUnlock(surface_, kIOSurfaceLockAvoidSync, nullptr);
  is_locked_ = false;
}

void IOSurfaceMac::Resize(uint32_t width, uint32_t height) {
  if (width_ == width && height_ == height)
    return;
    
  // Don't resize while locked
  if (is_locked_)
    return;
    
  // Create a new IOSurface with the new dimensions
  CreateIOSurface(width, height);
}

void IOSurfaceMac::MarkDirty(const ultralight::IntRect& dirtyRect) {
  if (!surface_)
    return;
    
  // Set dirty bounds in the Surface class
  set_dirty_bounds(dirtyRect);
  
  // Tell the system that we're done modifying the IOSurface
  // No need to call IOSurfaceFlushProcessorCaches as it's not available in all macOS versions
  // The locking/unlocking mechanism we use already ensures proper synchronization
}

IOSurfaceFactoryMac::IOSurfaceFactoryMac() {
}

IOSurfaceFactoryMac::~IOSurfaceFactoryMac() {
}

ultralight::Surface* IOSurfaceFactoryMac::CreateSurface(uint32_t width, uint32_t height) {
  return new IOSurfaceMac(width, height);
}

void IOSurfaceFactoryMac::DestroySurface(ultralight::Surface* surface) {
  delete static_cast<IOSurfaceMac*>(surface);
}