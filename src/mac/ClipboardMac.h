///
/// Copyright (C) 2020 Ultralight, Inc. All rights reserved.
///
#pragma once
#include <Ultralight/platform/Clipboard.h>

namespace ultralight {

class ClipboardMac : public Clipboard {
public:
  ClipboardMac() {}
  
  virtual void Clear() override;
  
  virtual String ReadPlainText() override;
  
  virtual void WritePlainText(const String& text) override;
};
  
}
