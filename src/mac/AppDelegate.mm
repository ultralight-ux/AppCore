#import "AppDelegate.h"
#include "AppMac.h"

@interface AppDelegate ()
@end

@implementation AppDelegate
{
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
}

-(BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
  return YES;
}

@end
