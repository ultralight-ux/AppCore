#pragma once
#include <Ultralight/platform/Surface.h>
#include <Windows.h>

namespace ultralight {

class DIBSurface final : public Surface {
public:
  DIBSurface(HDC window_dc, uint32_t width, uint32_t height);
  ~DIBSurface();

  virtual uint32_t width() const override;

  virtual uint32_t height() const override;

  virtual uint32_t row_bytes() const override;

  virtual size_t size() const override;

  virtual void* LockPixels() override;

  virtual void UnlockPixels() override;

  virtual void Resize(uint32_t width, uint32_t height) override;

  HDC dc() { return dc_; }

  HBITMAP bitmap() { return bitmap_; }

protected:
  void CreateBitmap(uint32_t width, uint32_t height);

  HDC dc_ = nullptr;
  HBITMAP bitmap_ = nullptr;
  void* bits_ = nullptr;
  uint32_t width_, height_, row_bytes_;
  size_t size_;
};

class DIBSurfaceFactory final : public SurfaceFactory {
public:
  DIBSurfaceFactory(HDC window_dc) : window_dc_(window_dc) {}

  virtual ~DIBSurfaceFactory() {}

  virtual Surface* CreateSurface(uint32_t width, uint32_t height) { return new DIBSurface(window_dc_, width, height); }

  virtual void DestroySurface(Surface* surface) { delete static_cast<DIBSurface*>(surface); }

protected:
  HDC window_dc_;
};

}  // namespace ultralight
