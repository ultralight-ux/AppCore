#pragma once
#include <Ultralight/platform/Clipboard.h>

namespace ultralight {

class ClipboardWin : public Clipboard {
public:
  ClipboardWin() {}

  virtual void Clear() override;

  virtual String ReadPlainText() override;

  virtual void WritePlainText(const String& text) override;
};

}  // namespace ultralight
