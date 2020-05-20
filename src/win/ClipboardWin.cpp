#include "ClipboardWin.h"
#include <Windows.h>

namespace ultralight {

void ClipboardWin::Clear() {
  EmptyClipboard();
}

String16 ClipboardWin::ReadPlainText() {
  if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
    return String16();

  if (!OpenClipboard(0))
    return String16();

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

void ClipboardWin::WritePlainText(const String16& text) {
  if (text.empty())
    return;

  if (!OpenClipboard(0))
    return;

  EmptyClipboard();

  HGLOBAL globalMem = GlobalAlloc(GMEM_MOVEABLE, (text.size() + 1) * sizeof(Char16));
  LPWSTR strMem = (LPWSTR)GlobalLock(globalMem);
  memcpy(strMem, text.data(), text.size() * sizeof(Char16));
  strMem[text.size()] = 0; // null character
  GlobalUnlock(globalMem);

  SetClipboardData(CF_UNICODETEXT, globalMem);

  CloseClipboard();
}

}  // namespace ultralight