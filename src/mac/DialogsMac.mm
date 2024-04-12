/******************************************************************************
 * This file is a part of Ultralight, an ultra-portable web-browser engine.  *
 *                                                                            *
 * See <https://ultralig.ht> for licensing and more.                          *
 *                                                                            *
 * (C) 2024 Ultralight, Inc.                                                  *
 ******************************************************************************/

#include <AppCore/Dialogs.h>
#import <Cocoa/Cocoa.h>

namespace ultralight {

ButtonResult ShowMessageBox(const String& title, const String& message, DialogIcon icon, ButtonType buttons) {
    NSString* nsTitle = [NSString stringWithUTF8String:title.utf8().data()];
    NSString* nsMessage = [NSString stringWithUTF8String:message.utf8().data()];
    
    NSAlert* alert = [[NSAlert alloc] init];
    [alert setMessageText:nsTitle];
    [alert setInformativeText:nsMessage];
    
    switch (icon) {
        case DialogIcon::Info:
            [alert setAlertStyle:NSAlertStyleInformational];
            break;
        case DialogIcon::Warning:
            [alert setAlertStyle:NSAlertStyleWarning];
            break;
        case DialogIcon::Error:
            [alert setAlertStyle:NSAlertStyleCritical];
            break;
        case DialogIcon::Question:
            [alert setAlertStyle:NSAlertStyleInformational];
            break;
    }
    
    switch (buttons) {
        case ButtonType::OK:
            [alert addButtonWithTitle:@"OK"];
            break;
        case ButtonType::OKCancel:
            [alert addButtonWithTitle:@"OK"];
            [alert addButtonWithTitle:@"Cancel"];
            break;
        case ButtonType::YesNo:
            [alert addButtonWithTitle:@"Yes"];
            [alert addButtonWithTitle:@"No"];
            break;
    }
    
    NSModalResponse response = [alert runModal];
    
    switch (response) {
        case NSAlertFirstButtonReturn:
            return buttons == ButtonType::YesNo ? ButtonResult::Yes : ButtonResult::OK;
        case NSAlertSecondButtonReturn:
            return buttons == ButtonType::YesNo ? ButtonResult::No : ButtonResult::Cancel;
        default:
            return ButtonResult::Cancel;
    }
}

} // namespace ultralight