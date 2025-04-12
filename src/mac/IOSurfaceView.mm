#import "IOSurfaceView.h"
#import <QuartzCore/QuartzCore.h>

@implementation IOSurfaceView {
  IOSurfaceRef _ioSurface;
  float _scale;
  CALayer* _contentLayer;
}

- (id)initWithFrame:(NSRect)frame 
       initialScale:(float)scale 
         ioSurface:(IOSurfaceRef)ioSurface {
  if (!(self = [super initWithFrame:frame]))
    return nil;
    
  _scale = scale;
  
  // Configure the view for layer-backing
  self.wantsLayer = YES;
  
  // Create content layer
  _contentLayer = [CALayer layer];
  _contentLayer.contentsScale = _scale;
  _contentLayer.contentsGravity = kCAGravityTopLeft;
  
  // Set the layer as the view's backing layer
  self.layer = _contentLayer;
  
  // Set IOSurface if provided
  if (ioSurface) {
    [self setIOSurface:ioSurface];
  }
  
  return self;
}

- (void)dealloc {
  _ioSurface = NULL;
}

- (void)setIOSurface:(IOSurfaceRef)ioSurface {
  _ioSurface = ioSurface;
  
  if (_ioSurface) {
    // Create a Core Animation IOSurface content
    _contentLayer.contents = (__bridge id)_ioSurface;
  } else {
    _contentLayer.contents = nil;
  }
}

- (void)setNeedsDisplayInRect:(NSRect)dirtyRect {
  // Force the layer to redraw the specified region
  [_contentLayer setNeedsDisplayInRect:dirtyRect];
}

@end