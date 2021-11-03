#pragma once
#include <Ultralight/platform/FontLoader.h>
#include <map>

namespace ultralight {
    
class FontLoaderLinux : public FontLoader {
public:
    FontLoaderLinux();
    virtual ~FontLoaderLinux();
    virtual String fallback_font() const override;
    virtual String fallback_font_for_characters(const String& characters, int weight, bool italic) const override;
    virtual RefPtr<FontFile> Load(const String& family, int weight, bool italic) override;
protected:
    std::map<uint64_t, RefPtr<Buffer>> fonts_;
}; 

}  // namespace ultralight
