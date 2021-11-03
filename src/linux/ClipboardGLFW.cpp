#include "ClipboardGLFW.h"
#include <Ultralight/String.h>
#include <GLFW/glfw3.h>

namespace ultralight {

ClipboardGLFW::ClipboardGLFW() {}

void ClipboardGLFW::Clear() {
}

String ClipboardGLFW::ReadPlainText() {
    if (window_) {
        return glfwGetClipboardString(window_);
    }

    return String();
}
  
void ClipboardGLFW::WritePlainText(const String& text) {
    if (window_) {
        glfwSetClipboardString(window_, text.utf8().data());
    }
}

}  // namespace ultralight
