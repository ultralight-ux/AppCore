#pragma once
#include <Ultralight/platform/Clipboard.h>

namespace ultralight {

class ClipboardWin : public Clipboard {
public:
  ClipboardWin() {}

  virtual void Clear() override;

  virtual String16 ReadPlainText() override;

  virtual void WritePlainText(const String16& text) override;
};

}  // namespace ultralight
