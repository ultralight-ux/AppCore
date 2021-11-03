#pragma once
#include <Ultralight/platform/Clipboard.h>

typedef struct GLFWwindow GLFWwindow;

namespace ultralight {

class ClipboardGLFW : public Clipboard {
public:
    ClipboardGLFW();

    void set_window(GLFWwindow* window) { window_ = window; }

    virtual void Clear() override;

    virtual String ReadPlainText() override;
  
    virtual void WritePlainText(const String& text) override;

protected:
    GLFWwindow* window_ = nullptr;
};

}  // namespace ultralight
