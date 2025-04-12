#pragma once
#import <AppKit/AppKit.h>
#import <IOSurface/IOSurface.h>

///
/// NSView subclass that displays an IOSurface.
///
@interface IOSurfaceView : NSView

// Initialize with an IOSurface
- (id)initWithFrame:(NSRect)frame 
       initialScale:(float)scale 
         ioSurface:(IOSurfaceRef)ioSurface;

// Update the backing IOSurface
- (void)setIOSurface:(IOSurfaceRef)ioSurface;

// Mark a region of the view as needing display
- (void)setNeedsDisplayInRect:(NSRect)dirtyRect;

@end