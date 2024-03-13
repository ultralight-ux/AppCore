#include "MonitorWin.h"
#include "DisplayLinkWin.h"
#include <Ultralight/Renderer.h>

namespace ultralight {

MonitorWin::MonitorWin(WindowsUtil* util)
    : util_(util)
{
    static uint32_t next_display_id = 0;
    display_id_ = next_display_id++;

    monitor_ = MonitorFromPoint({ 1, 1 }, MONITOR_DEFAULTTONEAREST);
    display_link_.reset(new DisplayLinkWin(monitor_, std::bind(&MonitorWin::OnDisplayRefresh, this)));
}

MonitorWin::~MonitorWin()
{
    display_link_.reset();
}

void MonitorWin::OnDisplayRefresh()
{
    // WARNING: This callback is invoked on a child-thread, make sure to use thread-safe primitives
    needs_refresh_ = true;
}

double MonitorWin::scale() const
{
    return util_->GetMonitorDPI(monitor_);
}

uint32_t MonitorWin::width() const
{
    MONITORINFO info;
    info.cbSize = sizeof(info);
    if (!GetMonitorInfo(monitor_, &info))
        MessageBoxW(NULL, (LPCWSTR)L"GetMonitorInfo failed", (LPCWSTR)L"Notification", MB_OK);

    return (uint32_t)abs(info.rcMonitor.left - info.rcMonitor.right);
}

uint32_t MonitorWin::height() const
{
    MONITORINFO info;
    info.cbSize = sizeof(info);
    if (!GetMonitorInfo(monitor_, &info))
        MessageBoxW(NULL, (LPCWSTR)L"GetMonitorInfo failed", (LPCWSTR)L"Notification", MB_OK);

    return (uint32_t)abs(info.rcMonitor.top - info.rcMonitor.bottom);
}

uint32_t MonitorWin::refresh_rate() const
{
    MONITORINFOEX mi;
    mi.cbSize = sizeof(MONITORINFOEX);

    // Attempt to get monitor information
    if (GetMonitorInfo(monitor_, &mi)) {
        DEVMODE dm;
        ZeroMemory(&dm, sizeof(dm));
        dm.dmSize = sizeof(dm);

        // Enumerate display settings for the specific monitor
        if (EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &dm)) {
            return dm.dmDisplayFrequency; // Return the actual refresh rate
        }
    }
    return 60; // Fallback to 60 Hz if unable to get the information
}

} // namespace ultralight
