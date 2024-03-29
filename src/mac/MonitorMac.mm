#import "MonitorMac.h"
#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>

namespace ultralight {

MonitorMac::MonitorMac()
{
    screen_ = [[NSScreen screens] firstObject];
    static uint32_t next_display_id = 0;
    display_id_ = next_display_id++;
}

double MonitorMac::scale() const
{
    return [screen_ backingScaleFactor];
}

uint32_t MonitorMac::width() const
{
    return (uint32_t)screen_.frame.size.width;
}

uint32_t MonitorMac::height() const
{
    return (uint32_t)screen_.frame.size.height;
}

uint32_t MonitorMac::refresh_rate() const
{
    // Retrieve the NSScreen's device description dictionary
    NSDictionary* screenDictionary = [screen_ deviceDescription];

    // Extract the screenNumber from the device description
    NSNumber* screenNumber = [screenDictionary objectForKey:@"NSScreenNumber"];

    // Convert NSNumber to CGDirectDisplayID
    CGDirectDisplayID displayID = [screenNumber unsignedIntValue];

    // Use CGDisplay to obtain the refresh rate
    CGDisplayModeRef currentMode = CGDisplayCopyDisplayMode(displayID);
    double refreshRate = CGDisplayModeGetRefreshRate(currentMode);

    // If the system reports a 0 refresh rate, it's likely an LCD
    // that uses the default refresh rate of 60Hz.
    if (refreshRate == 0) {
        refreshRate = 60.0;
    }

    CGDisplayModeRelease(currentMode);

    return static_cast<uint32_t>(refreshRate);
}

} // namespace ultralight
