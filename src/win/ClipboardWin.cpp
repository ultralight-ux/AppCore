#include "ClipboardWin.h"
#include <Windows.h>

namespace ultralight {

void ClipboardWin::Clear() {
  EmptyClipboard();
}

String ClipboardWin::ReadPlainText() {
  if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
    return String();

  if (!OpenClipboard(0))
    return String();

  String16 result;

  HGLOBAL globalMem = GetClipboardData(CF_UNICODETEXT);
  if (globalMem) {
    LPWSTR strMem = (LPWSTR)GlobalLock(globalMem);
    if (strMem) {
      result = String16(strMem, lstrlenW(strMem));
      GlobalUnlock(globalMem);
    }
  }

  CloseClipboard();

  return result;
}

void ClipboardWin::WritePlainText(const String& text) {
  if (text.empty())
    return;

  if (!OpenClipboard(0))
    return;

  EmptyClipboard();

  String16 text16 = text.utf16();

  HGLOBAL globalMem = GlobalAlloc(GMEM_MOVEABLE, (text16.size() + 1) * sizeof(Char16));
  if (globalMem) {
    LPWSTR strMem = (LPWSTR)GlobalLock(globalMem);
    if (strMem) {
      memcpy(strMem, text16.data(), text16.size() * sizeof(Char16));
      strMem[text16.size()] = 0; // null character
      GlobalUnlock(globalMem);
      SetClipboardData(CF_UNICODETEXT, globalMem);
    }
  }

  CloseClipboard();
}

}  // namespace ultralight