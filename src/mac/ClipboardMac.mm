#include "ClipboardMac.h"
#import <AppKit/AppKit.h>
#include <vector>

namespace ultralight {
  
static NSString* ToNSString(const String16& str) {
  auto pathData = [[NSData alloc] initWithBytesNoCopy:const_cast<void*>(reinterpret_cast<const void*>(str.data()))
                                               length:str.length() * sizeof(Char16)
                                         freeWhenDone:NO];
  return [[NSString alloc] initWithData:pathData
                               encoding:NSUTF16LittleEndianStringEncoding];
}
  
static String16 ToString16(CFStringRef str) {
  if (!str)
    return String16();
  CFIndex size = CFStringGetLength(str);
  std::vector<Char16> buffer(size);
  CFStringGetCharacters(str, CFRangeMake(0, size), (UniChar*)buffer.data());
  return String16(buffer.data(), size);
}
  
void ClipboardMac::Clear() {
  NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  [pasteboard clearContents];
}

String16 ClipboardMac::ReadPlainText() {
  NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  NSString* str = [pasteboard stringForType:NSPasteboardTypeString];
  return ToString16((__bridge CFStringRef)str);
}

void ClipboardMac::WritePlainText(const String16& text) {
  NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  [pasteboard clearContents];
  [pasteboard declareTypes:[NSArray arrayWithObjects:NSPasteboardTypeString, nil] owner:nil];
  [pasteboard setString:ToNSString(text) forType:NSPasteboardTypeString];
}

}  // namespace ultralight
