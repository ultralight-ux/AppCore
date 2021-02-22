#import "ViewController.h"
#import <MetalKit/MetalKit.h>
#import "ViewDelegate.h"
#import "WindowMac.h"
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>
#import <dispatch/dispatch.h>
#import "AppMac.h"
#import "metal/GPUContextMetal.h"

@implementation MTLView
{
  float initialScale;
}

- (id)initWithFrame:(NSRect)frame initialScale:(float)scale
{
  if (!(self = [super initWithFrame:frame])) {
    return self;
  }
  
  initialScale = scale;
  
  // We want to be backed by a CAMetalLayer.
  self.wantsLayer = YES;
  self.layerContentsRedrawPolicy = NSViewLayerContentsRedrawOnSetNeedsDisplay;
  self.layerContentsPlacement = NSViewLayerContentsPlacementTopLeft;
  
  return self;
}

- (CALayer*)makeBackingLayer
{
  ultralight::AppMac* app = static_cast<ultralight::AppMac*>(ultralight::App::instance());
  CAMetalLayer* metalLayer = [CAMetalLayer layer];
  metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
  metalLayer.device = app->gpu_context()->device();
  metalLayer.allowsNextDrawableTimeout = false;
  metalLayer.autoresizingMask = kCALayerHeightSizable | kCALayerWidthSizable;
  metalLayer.needsDisplayOnBoundsChange = YES;
  metalLayer.contentsScale = initialScale;
    
  return metalLayer;
}

- (CAMetalLayer*)metalLayer
{
  return (CAMetalLayer*)self.layer;
}

- (void)setDelegate:(ViewDelegate*)delegate
{
  self.metalLayer.delegate = delegate;
}

- (void)setFrameSize:(NSSize)newSize
{
  [super setFrameSize:newSize];
  if (self.window)
    self.metalLayer.contentsScale = self.window.backingScaleFactor;
  newSize.width *= self.metalLayer.contentsScale;
  newSize.height *= self.metalLayer.contentsScale;
  self.metalLayer.drawableSize = newSize;
  [self viewDidChangeBackingProperties];
  [(ViewDelegate*)self.metalLayer.delegate resizeLayer:newSize];
}

- (void)viewDidChangeBackingProperties
{
  // TODO: handle change in contentsScale/backingScaleFactor here.
}

- (BOOL)acceptsFirstResponder {
  return YES;
}

- (BOOL)performKeyEquivalent:(NSEvent *)event {
  if ([[NSApp mainMenu] performKeyEquivalent:event])
    return YES;
  
  // Suppress the "doonk" sound when keyboard shortcuts are used on NSView
  [(ViewDelegate*)self.metalLayer.delegate keyEvent:event];
  return YES;
}

@end

@implementation ViewController
{
  ultralight::WindowMac* _window;
  NSRect _initialFrame;
  float _initialScale;
  ViewDelegate *_delegate;
}

- (void)initWithWindow:(ultralight::WindowMac*)window frame:(NSRect)rect
{
  _window = window;
  _initialFrame = rect;
  _initialScale = window->scale();
}

- (MTLView*)metalView
{
  return (MTLView*)self.view;
}

- (void)loadView
{
  MTLView* view = [[MTLView alloc] initWithFrame:_initialFrame
                                    initialScale:_initialScale];
  [self setView:view];
}

- (void)viewDidLayout
{
  NSTrackingArea* trackingArea = [[NSTrackingArea alloc]
                                  initWithRect:self.view.bounds
                                  options:NSTrackingMouseMoved | NSTrackingActiveAlways
                                  owner:self userInfo:nil];
  [self.view addTrackingArea:trackingArea];
}

- (void)viewDidLoad
{
  [super viewDidLoad];

  if (!self.metalView.metalLayer.device)
  {
    NSLog(@"Metal is not supported on this device");
    return;
  }

  _delegate = [[ViewDelegate alloc] initWithWindow:_window];

  if (!_delegate)
  {
    NSLog(@"Application failed initialization");
    return;
  }

  [(MTLView*)self.view setDelegate:_delegate];

  NSTrackingArea* trackingArea = [[NSTrackingArea alloc]
                                  initWithRect:self.view.bounds
                                  options:NSTrackingMouseMoved | NSTrackingActiveAlways
                                  owner:self userInfo:nil];
  [self.view addTrackingArea:trackingArea];
}

- (void)scrollWheel:(NSEvent *)event
{
  NSPoint eventLocation = [event locationInWindow];
  NSPoint point = [self.view convertPoint:eventLocation fromView:nil];
  point.y = self.view.bounds.size.height - point.y;
  
  [_delegate scrollWheel:[event deltaY] * 8 mouseX:point.x mouseY:point.y];
}

- (void)mouseMoved:(NSEvent *)event
{
  NSPoint eventLocation = [event locationInWindow];
  NSPoint point = [self.view convertPoint:eventLocation fromView:nil];
  point.y = self.view.bounds.size.height - point.y;
  
  [_delegate mouseMoved:point.x mouseY:point.y];
}

- (void)mouseDragged:(NSEvent *)event
{
  NSPoint eventLocation = [event locationInWindow];
  NSPoint point = [self.view convertPoint:eventLocation fromView:nil];
  point.y = self.view.bounds.size.height - point.y;
  
  [_delegate mouseMoved:point.x mouseY:point.y];
}

- (void)mouseDown:(NSEvent *)event
{
  NSPoint eventLocation = [event locationInWindow];
  NSPoint point = [self.view convertPoint:eventLocation fromView:nil];
  point.y = self.view.bounds.size.height - point.y;
  
  [_delegate leftMouseDown:point.x mouseY:point.y];
}

- (void)mouseUp:(NSEvent *)event
{
  NSPoint eventLocation = [event locationInWindow];
  NSPoint point = [self.view convertPoint:eventLocation fromView:nil];
  point.y = self.view.bounds.size.height - point.y;
  
  [_delegate leftMouseUp:point.x mouseY:point.y];
}

- (void)rightMouseDown:(NSEvent *)event
{
  NSPoint eventLocation = [event locationInWindow];
  NSPoint point = [self.view convertPoint:eventLocation fromView:nil];
  point.y = self.view.bounds.size.height - point.y;
  
  [_delegate rightMouseDown:point.x mouseY:point.y];
}

- (void)rightMouseUp:(NSEvent *)event
{
  NSPoint eventLocation = [event locationInWindow];
  NSPoint point = [self.view convertPoint:eventLocation fromView:nil];
  point.y = self.view.bounds.size.height - point.y;
  
  [_delegate rightMouseUp:point.x mouseY:point.y];
}

- (void)keyUp:(NSEvent *)event
{
  [_delegate keyEvent:event];
}

- (void)keyDown:(NSEvent *)event
{
  [_delegate keyEvent:event];
  [self interpretKeyEvents:[NSArray arrayWithObject:event]];
}

- (void)insertText:(id)string {
  [_delegate textEvent:string];
}

- (void) deleteBackward: (id) sender {
}

- (void) insertNewline: (id) sender {
  [self insertText: @"\r"];
}

- (void) insertTab: (id) sender {
  [self insertText: @"\t"];
}

- (void) moveLeft: (id) sender {
}

- (void) moveRight: (id) sender {
}

- (void) moveUp: (id) sender {
}

- (void) moveDown: (id) sender {
}

- (void)setRepresentedObject:(id)representedObject {
  [super setRepresentedObject:representedObject];
}

@end

