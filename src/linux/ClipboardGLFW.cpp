#include "ClipboardGLFW.h"
#include <Ultralight/String.h>
#include <GLFW/glfw3.h>

namespace ultralight {

ClipboardGLFW::ClipboardGLFW() {}

void ClipboardGLFW::Clear() {
}

String16 ClipboardGLFW::ReadPlainText() {
    if (window_) {
        return String16(glfwGetClipboardString(window_));
    }

    return String16();
}
  
void ClipboardGLFW::WritePlainText(const String16& text) {
    if (window_) {
        glfwSetClipboardString(window_, String(text).utf8().data());
    }
}

}  // namespace ultralight
