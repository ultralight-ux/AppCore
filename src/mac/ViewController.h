#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

namespace ultralight { class WindowMac; }

@interface MTLView : NSView
- (CAMetalLayer*)metalLayer;
@end

@interface ViewController : NSViewController

- (void)initWithWindow:(ultralight::WindowMac*)window frame:(NSRect)rect;
- (MTLView*)metalView;

@end
