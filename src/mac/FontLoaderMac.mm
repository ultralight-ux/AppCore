#import "FontLoaderMac.h"
#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#import <CoreText/CoreText.h>
#include "FallbackFont.h"
#include <vector>

namespace ultralight {

FontLoaderMac::FontLoaderMac() {}

FontLoaderMac::~FontLoaderMac() {}
    
String FontLoaderMac::fallback_font() const {
    return "Fallback-Font-Roboto";
}
    
static String16 ToString16(CFStringRef str) {
    if (!str)
        return String16();
    CFIndex size = CFStringGetLength(str);
    std::vector<Char16> buffer(size);
    CFStringGetCharacters(str, CFRangeMake(0, size), (UniChar*)buffer.data());
    return String16(buffer.data(), size);
}

String FontLoaderMac::fallback_font_for_characters(const String& characters, int weight, bool italic) const {
    String16 characters16 = characters.utf16();
    CFStringRef str = CFStringCreateWithCharacters(kCFAllocatorDefault, reinterpret_cast<const UniChar*>(characters16.data()), characters16.length());
    // Use Helvetica as our base font, go through its cascade list to find a system fallback that can display the given charachters
    CTFontRef font = CTFontCreateForString(CTFontCreateWithName(CFSTR("Helvetica"), 12.0, NULL), str, {0, (CFIndex)characters16.length()});
    CFStringRef family = CTFontCopyFamilyName(font);
    String16 result = ToString16(family);
    if (result.empty())
        return fallback_font();
    return result;
}

static NSString* ToNSString(const String16& str) {
    auto pathData = [[NSData alloc] initWithBytesNoCopy:const_cast<void*>(reinterpret_cast<const void*>(str.data()))
                                                 length:str.length() * sizeof(Char16)
                                           freeWhenDone:NO];
    return [[NSString alloc] initWithData:pathData
                                 encoding:NSUTF16LittleEndianStringEncoding];
}
    
static bool Equals(const String16& a, const String16& b) {
    if (a.length() != b.length())
        return false;
    
    for (size_t i = 0; i < a.length(); ++i)
        if (a.data()[i] != b.data()[i])
            return false;
    
    return true;
}
    
#define kCTFontWeightUltraLight -0.8
#define kCTFontWeightThin -0.6
#define kCTFontWeightLight -0.4
#define kCTFontWeightRegular 0
#define kCTFontWeightMedium 0.23
#define kCTFontWeightSemibold 0.3
#define kCTFontWeightBold 0.4
#define kCTFontWeightHeavy 0.56
#define kCTFontWeightBlack 0.62

static NSDictionary* createFontAttributes(NSString* family, int weight, bool italic, float size)
{
    NSMutableDictionary* traitsDictionary = [[NSMutableDictionary alloc] init];
    
    float ctWeight = kCTFontWeightRegular;
    if (weight < 150)
        ctWeight = kCTFontWeightUltraLight;
    else if (weight < 250)
        ctWeight = kCTFontWeightThin;
    else if (weight < 350)
        ctWeight = kCTFontWeightLight;
    else if (weight < 450)
        ctWeight = kCTFontWeightRegular;
    else if (weight < 550)
        ctWeight = kCTFontWeightMedium;
    else if (weight < 650)
        ctWeight = kCTFontWeightSemibold;
    else if (weight < 750)
        ctWeight = kCTFontWeightBold;
    else if (weight < 850)
        ctWeight = kCTFontWeightHeavy;
    else
        ctWeight = kCTFontWeightBlack;
    
    [traitsDictionary setObject:[NSNumber numberWithFloat:ctWeight] forKey:static_cast<NSString *>(kCTFontWeightTrait)];
    
    if (italic)
        [traitsDictionary setObject:[NSNumber numberWithInt:kCTFontItalicTrait] forKey:static_cast<NSString *>(kCTFontSymbolicTrait)];
    
    return @{
             (NSString *)kCTFontFamilyNameAttribute : family,
             (NSString *)kCTFontSizeAttribute : [NSNumber numberWithFloat:size],
             (NSString*)kCTFontTraitsAttribute : traitsDictionary };
}

RefPtr<FontFile> FontLoaderMac::Load(const String& family, int weight, bool italic) {
    String16 family_name = family.utf16();
    if (Equals(family_name, String("-apple-system").utf16()))
        family_name = String(".AppleSystemUIFont").utf16();

    if (Equals(family_name, fallback_font().utf16())) {
        // Use embedded font file as the last resort fallback
        // We don't specify a destruction callback since the bytes are embedded in the binary.
        auto buffer = Buffer::Create((void*)const_cast<unsigned char*>(&Roboto_Regular_ttf[0]), 
            sizeof(Roboto_Regular_ttf), nullptr, nullptr);
        return FontFile::Create(buffer);
    }
    
    CTFontDescriptorRef descriptor = CTFontDescriptorCreateWithAttributes(
      (CFDictionaryRef)createFontAttributes(ToNSString(family_name), weight, italic, 12.0));
    CFURLRef url = (CFURLRef)CTFontDescriptorCopyAttribute(descriptor, kCTFontURLAttribute);
    
    if (!url)
        return nullptr;
    
    NSString* fontPath = [NSString stringWithString:[(__bridge NSURL *)url path]];
    CFRelease(descriptor);
  
    ultralight::String8 filepath([fontPath UTF8String]);
    return FontFile::Create(filepath);
}

// Called from Platform.cpp
FontLoader* CreatePlatformFontLoader() {
    return new FontLoaderMac();
}

}  // namespace ultralight

