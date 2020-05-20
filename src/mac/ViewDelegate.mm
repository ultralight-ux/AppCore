#import "ViewDelegate.h"
#include <Ultralight/String.h>
#include <Ultralight/MouseEvent.h>
#include <AppCore/App.h>
#include "AppMac.h"
#include "WindowMac.h"
#include "OverlayManager.h"

static inline ultralight::String toString(NSString* str) {
  CFStringRef cfStr = (__bridge CFStringRef)str;
  if (!cfStr)
    return ultralight::String();
  CFIndex size = CFStringGetLength(cfStr);
  std::vector<ultralight::Char16> buffer(size);
  CFStringGetCharacters(cfStr, CFRangeMake(0, size), (UniChar*)buffer.data());
  return ultralight::String16(buffer.data(), size);
}

@implementation ViewDelegate
{
  ultralight::WindowMac* _window;
  ultralight::MouseEvent::Button _cur_button;
}

- (nonnull instancetype)initWithWindow:(ultralight::WindowMac*)window
{
  self = [super init];
  if (self)
    _window = window;
  
  _cur_button = ultralight::MouseEvent::kButton_None;
  
  return self;
}

- (void)displayLayer:(CALayer *)layer
{
  ultralight::AppMac* app = static_cast<ultralight::AppMac*>(ultralight::App::instance());
  app->OnPaint((CAMetalLayer*)layer);
}

- (void)mouseMoved:(CGFloat)x mouseY:(CGFloat)y
{
  _window->FireMouseEvent({ ultralight::MouseEvent::kType_MouseMoved,
    (int)x, (int)y, _cur_button });
}

- (void)leftMouseDown:(CGFloat)x mouseY:(CGFloat)y
{
  _window->FireMouseEvent({ ultralight::MouseEvent::kType_MouseDown,
    (int)x, (int)y, ultralight::MouseEvent::kButton_Left });
  _cur_button = ultralight::MouseEvent::kButton_Left;
}

- (void)leftMouseUp:(CGFloat)x mouseY:(CGFloat)y
{
  _window->FireMouseEvent({ ultralight::MouseEvent::kType_MouseUp,
    (int)x, (int)y, ultralight::MouseEvent::kButton_Left });
  _cur_button = ultralight::MouseEvent::kButton_None;
}

- (void)rightMouseDown:(CGFloat)x mouseY:(CGFloat)y
{
  _window->FireMouseEvent({ ultralight::MouseEvent::kType_MouseDown,
    (int)x, (int)y, ultralight::MouseEvent::kButton_Right });
  _cur_button = ultralight::MouseEvent::kButton_Right;
}

- (void)rightMouseUp:(CGFloat)x mouseY:(CGFloat)y
{
  _window->FireMouseEvent({ ultralight::MouseEvent::kType_MouseUp,
    (int)x, (int)y, ultralight::MouseEvent::kButton_Right });
  _cur_button = ultralight::MouseEvent::kButton_None;
}

- (void)scrollWheel:(CGFloat)deltaY mouseX:(CGFloat)x mouseY:(CGFloat)y
{
  _window->FireScrollEvent({ ultralight::ScrollEvent::kType_ScrollByPixel, 0, (int)deltaY });
}

- (void)keyEvent:(NSEvent*)event
{
  _window->FireKeyEvent(ultralight::KeyEvent(event));
}

- (void)textEvent:(id)string
{
  ultralight::KeyEvent charEvent;
  charEvent.type = ultralight::KeyEvent::kType_Char;
  charEvent.text = toString(string);
  charEvent.unmodified_text = toString(string);
  
  _window->FireKeyEvent(charEvent);
}

- (void)resizeLayer:(NSSize)size
{
  _window->OnResize(size.width, size.height);
}

@end

