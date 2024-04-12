/**********************************************************************
 * This file is a part of Ultralight, an ultra-portable web-browser engine.
 * See <https://ultralig.ht> for licensing and more.
 * (C) 2024 Ultralight, Inc.
 **********************************************************************/

#include <Windows.h>
#include <AppCore/Dialogs.h>
#include <ShellScalingAPI.h>

namespace ultralight {

void SetDPIAwareness()
{
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
}

ButtonResult ShowMessageBox(const String& title, const String& message, DialogIcon icon, ButtonType buttons)
{
    SetDPIAwareness();

    UINT type = MB_ICONINFORMATION;
    switch (icon) {
    case DialogIcon::Info:
        type = MB_ICONINFORMATION;
        break;
    case DialogIcon::Warning:
        type = MB_ICONWARNING;
        break;
    case DialogIcon::Error:
        type = MB_ICONERROR;
        break;
    case DialogIcon::Question:
        type = MB_ICONQUESTION;
        break;
    }

    switch (buttons) {
    case ButtonType::OKCancel:
        type |= MB_OKCANCEL;
        break;
    case ButtonType::YesNo:
        type |= MB_YESNO;
        break;
    default:
        type |= MB_OK;
        break;
    }

    int result = MessageBoxW(nullptr, message.utf16().data(), title.utf16().data(), type);

    switch (result) {
    case IDOK:
        return ButtonResult::OK;
    case IDCANCEL:
        return ButtonResult::Cancel;
    case IDYES:
        return ButtonResult::Yes;
    case IDNO:
        return ButtonResult::No;
    default:
        return ButtonResult::Cancel;
    }
}

} // namespace ultralight