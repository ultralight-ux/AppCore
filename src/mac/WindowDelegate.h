#import <Cocoa/Cocoa.h>

namespace ultralight { class WindowMac; }

@interface WindowDelegate : NSObject <NSWindowDelegate>

- (void)initWithWindow:(ultralight::WindowMac*)window;

- (void)windowWillClose:(NSNotification *)notification;

- (void)windowDidBecomeKey:(NSNotification *)notification;

- (void)windowDidResignKey:(NSNotification *)notification;

@end
