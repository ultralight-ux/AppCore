#pragma once
#include <Ultralight/platform/FontLoader.h>
#include <map>

namespace ultralight {

/**
 * FontLoader implementation for Windows.
 */
class FontLoaderWin : public FontLoader {
public:
  FontLoaderWin() {}
  virtual ~FontLoaderWin() {}
  virtual String16 fallback_font() const override;
  virtual String16 fallback_font_for_characters(const String16& characters, int weight, bool italic) const override;
  virtual RefPtr<FontFile> Load(const String16& family, int weight, bool italic) override;
protected:
  std::map<uint32_t, RefPtr<Buffer>> fonts_;
};

}  // namespace ultralight
