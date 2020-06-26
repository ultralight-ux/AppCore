#include "DIBSurface.h"
#include <Windows.h>

namespace ultralight {

DIBSurface::DIBSurface(HDC window_dc, uint32_t width, uint32_t height) {
  dc_ = CreateCompatibleDC(window_dc);

  CreateBitmap(width, height);
}

DIBSurface::~DIBSurface() {
  if (bitmap_)
    DeleteObject(bitmap_);
  DeleteDC(dc_);
}

uint32_t DIBSurface::width() const { return width_; }

uint32_t DIBSurface::height() const { return height_; }

uint32_t DIBSurface::row_bytes() const { return row_bytes_; }

size_t DIBSurface::size() const { return size_; }

void* DIBSurface::LockPixels() { return bits_; }

void DIBSurface::UnlockPixels() { }

void DIBSurface::Resize(uint32_t width, uint32_t height) {
  if (width != width_ || height != height_)
    CreateBitmap(width, height);
}

void DIBSurface::CreateBitmap(uint32_t width, uint32_t height) {
  if (bitmap_)
    DeleteObject(bitmap_);

  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(BITMAPINFO));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = static_cast<LONG>(width);
  bmi.bmiHeader.biHeight = -static_cast<LONG>(height);
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = width * height * 4;

  bitmap_ = CreateDIBSection(dc_, &bmi, DIB_RGB_COLORS, &bits_, NULL, 0x0);
  SelectObject(dc_, bitmap_);
  width_ = width;
  height_ = height;
  row_bytes_ = width * 4;
  size_ = static_cast<size_t>(row_bytes_) * height_;
}

}  // namespace ultralight
