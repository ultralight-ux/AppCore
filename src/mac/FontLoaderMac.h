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
    virtual String16 fallback_font() const override;
    virtual String16 fallback_font_for_characters(const String16& characters, int weight, bool italic) const override;
    virtual RefPtr<FontFile> Load(const String16& family, int weight, bool italic) override;
};
    
}  // namespace ultralight
