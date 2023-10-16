#pragma once
#include <Ultralight/platform/Surface.h>

///
/// Custom Surface implementation that allows the CPU renderer to paint directly
/// into an Ultralight GPU texture.
///
class ULTextureSurface : public ultralight::Surface {
public:
  virtual ~ULTextureSurface() {}
  
  //
  // Get the Ultralight texture handle (may be 0 if not synchronized yet)
  //
  virtual uint32_t texture_id() const = 0;

  //
  // Whether or not the backing bitmap needs to be uploaded to the texture.
  //
  virtual bool NeedsSynchronize() const = 0;

  //
  // Sync the texture (upload bitmap to GPU texture), returns true if texture was changed.
  //
  virtual bool Synchronize() = 0;
};

class ULTextureSurfaceFactory : public ultralight::SurfaceFactory {
public:
  ULTextureSurfaceFactory();

  virtual ~ULTextureSurfaceFactory();

  virtual ultralight::Surface* CreateSurface(uint32_t width, uint32_t height) override;

  virtual void DestroySurface(ultralight::Surface* surface) override;
};
