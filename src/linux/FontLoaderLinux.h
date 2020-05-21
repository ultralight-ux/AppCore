#pragma once
#include <Ultralight/platform/FontLoader.h>
#include <map>

namespace ultralight {
    
class FontLoaderLinux : public FontLoader {
public:
    FontLoaderLinux();
    virtual ~FontLoaderLinux();
    virtual String16 fallback_font() const override;
    virtual String16 fallback_font_for_characters(const String16& characters, int weight, bool italic) const override;
    virtual RefPtr<FontFile> Load(const String16& family, int weight, bool italic) override;
protected:
    std::map<uint64_t, RefPtr<Buffer>> fonts_;
}; 

}  // namespace ultralight
