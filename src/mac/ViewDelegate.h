#import <MetalKit/MetalKit.h>
#import <QuartzCore/CAMetalLayer.h>

namespace ultralight { class WindowMac; }

@interface ViewDelegate : NSObject<CALayerDelegate>

- (nonnull instancetype)initWithWindow:(ultralight::WindowMac*)window;

- (void)mouseMoved:(CGFloat)x mouseY:(CGFloat)y;

- (void)leftMouseDown:(CGFloat)x mouseY:(CGFloat)y;

- (void)leftMouseUp:(CGFloat)x mouseY:(CGFloat)y;

- (void)rightMouseDown:(CGFloat)x mouseY:(CGFloat)y;

- (void)rightMouseUp:(CGFloat)x mouseY:(CGFloat)y;

- (void)scrollWheel:(CGFloat)deltaY mouseX:(CGFloat)x mouseY:(CGFloat)y;

- (void)keyEvent:(NSEvent*)event;

- (void)textEvent:(id)string;

- (void)resizeLayer:(NSSize)size;

@end

