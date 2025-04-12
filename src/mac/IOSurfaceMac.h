#pragma once
#include <Ultralight/platform/Surface.h>
#include <Ultralight/Geometry.h>
#include <IOSurface/IOSurface.h>
#include <CoreFoundation/CoreFoundation.h>

///
/// Custom Surface implementation that uses IOSurface for zero-copy
/// display of CPU-rendered content on macOS.
///
class IOSurfaceMac : public ultralight::Surface {
public:
  IOSurfaceMac(uint32_t width, uint32_t height);
  virtual ~IOSurfaceMac();

  // Surface interface implementation
  virtual uint32_t width() const override;
  virtual uint32_t height() const override;
  virtual uint32_t row_bytes() const override;
  virtual size_t size() const override;
  virtual void* LockPixels() override;
  virtual void UnlockPixels() override;
  virtual void Resize(uint32_t width, uint32_t height) override;
  
  // Access to the IOSurface
  IOSurfaceRef GetIOSurface() const { return surface_; }
  
  // Notify that the IOSurface has been modified
  void MarkDirty(const ultralight::IntRect& dirtyRect);

private:
  uint32_t width_;
  uint32_t height_;
  uint32_t row_bytes_;
  IOSurfaceRef surface_;
  void* pixels_;
  bool is_locked_;
  
  void CreateIOSurface(uint32_t width, uint32_t height);
  void DestroyIOSurface();
};

///
/// Factory to create IOSurfaceMac instances.
///
class IOSurfaceFactoryMac : public ultralight::SurfaceFactory {
public:
  IOSurfaceFactoryMac();
  virtual ~IOSurfaceFactoryMac();
  
  // SurfaceFactory interface implementation
  virtual ultralight::Surface* CreateSurface(uint32_t width, uint32_t height) override;
  virtual void DestroySurface(ultralight::Surface* surface) override;
};