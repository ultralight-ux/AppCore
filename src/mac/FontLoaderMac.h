///
/// Copyright (C) 2020 Ultralight, Inc. All rights reserved.
///
#pragma once
#include <Ultralight/platform/FontLoader.h>

namespace ultralight {
    
class FontLoaderMac : public FontLoader {
public:
    FontLoaderMac();
    virtual ~FontLoaderMac();
    virtual String fallback_font() const override;
    virtual String fallback_font_for_characters(const String& characters, int weight, bool italic) const override;
    virtual RefPtr<FontFile> Load(const String& family, int weight, bool italic) override;
};
    
}  // namespace ultralight
