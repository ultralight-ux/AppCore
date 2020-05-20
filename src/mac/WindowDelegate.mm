#import "WindowDelegate.h"
#include "WindowMac.h"

@interface WindowDelegate ()

@end

@implementation WindowDelegate
{
  ultralight::WindowMac* window_;
}

- (void)initWithWindow:(ultralight::WindowMac*)window {
  window_ = window;
}

- (void)windowWillClose:(NSNotification *)notification {
  window_->OnClose();
}

- (void)windowDidResize:(NSNotification *)notification {
}

- (void)windowWillStartLiveResize:(NSNotification *)notification {
}

- (void)windowDidEndLiveResize:(NSNotification *)notification {

}

- (void)windowDidBecomeKey:(NSNotification *)notification {
  window_->SetWindowFocused(true);
}

- (void)windowDidResignKey:(NSNotification *)notification {
  window_->SetWindowFocused(false);
}

@end
