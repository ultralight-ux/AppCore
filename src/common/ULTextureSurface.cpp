#include "ULTextureSurface.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/GPUDriver.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <string>

///
/// Custom Surface implementation that allows the CPU renderer to paint directly
/// into an Ultralight GPU texture.
///
class ULTextureSurfaceImpl : public ULTextureSurface {
public:
  ULTextureSurfaceImpl(uint32_t width, uint32_t height) {
    Resize(width, height);
  }

  virtual ~ULTextureSurfaceImpl() {
    DestroyTexture();
  }

  void DestroyTexture() {
    if (texture_id_ == 0)
      return;

    auto gpu_driver = ultralight::Platform::instance().gpu_driver();
    if (gpu_driver)
      gpu_driver->DestroyTexture(texture_id_);
    
    texture_id_ = 0;
  }

  virtual uint32_t width() const override { return bitmap_->width(); }

  virtual uint32_t height() const override { return bitmap_->height(); }

  virtual uint32_t row_bytes() const override { return bitmap_->row_bytes(); }

  virtual size_t size() const override { return bitmap_->size(); }

  virtual void* LockPixels() override { return bitmap_->LockPixels(); }

  virtual void UnlockPixels() override { return bitmap_->UnlockPixels(); }

  virtual void Resize(uint32_t width, uint32_t height) override {
    if (bitmap_ && bitmap_->width() == width && bitmap_->height() == height)
      return;

    if (bitmap_) {
      bitmap_ = nullptr;
      DestroyTexture();
    }

    bitmap_ = ultralight::Bitmap::Create(width, height, ultralight::BitmapFormat::BGRA8_UNORM_SRGB);
  }

  virtual uint32_t texture_id() const { return texture_id_; }
  
  virtual bool NeedsSynchronize() const {
    return !dirty_bounds().IsEmpty() || texture_id_ == 0;
  }

  virtual bool Synchronize() {
    auto gpu_driver = ultralight::Platform::instance().gpu_driver();
    if (!gpu_driver)
      return false;

    if (!NeedsSynchronize()) 
      return false;

    if (texture_id_ == 0) {
      texture_id_ = gpu_driver->NextTextureId();
      gpu_driver->CreateTexture(texture_id_, bitmap_);
    } else {
      gpu_driver->UpdateTexture(texture_id_, bitmap_);
    }

    // Clear dirty bounds
    ClearDirtyBounds();

    return true;
  }

protected:
  uint32_t texture_id_ = 0;
  ultralight::RefPtr<ultralight::Bitmap> bitmap_;
};

ULTextureSurfaceFactory::ULTextureSurfaceFactory() {
}

ULTextureSurfaceFactory::~ULTextureSurfaceFactory() {
}

ultralight::Surface* ULTextureSurfaceFactory::CreateSurface(uint32_t width, uint32_t height) {
  ///
  /// Called by Ultralight when it wants to create a Surface.
  ///
  return new ULTextureSurfaceImpl(width, height);
}

void ULTextureSurfaceFactory::DestroySurface(ultralight::Surface* surface) {
  ///
  /// Called by Ultralight when it wants to destroy a Surface.
  ///
  delete static_cast<ULTextureSurfaceImpl*>(surface);
}
