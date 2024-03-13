#pragma once
#include "WindowsUtil.h"
#include <AppCore/Monitor.h>
#include <Windows.h>
#include <atomic>
#include <memory>

class DisplayLinkWin;

namespace ultralight {

class MonitorWin : public Monitor {
public:
    MonitorWin(WindowsUtil* util);
    virtual ~MonitorWin();

    virtual uint32_t display_id() const override { return display_id_; }

    virtual double scale() const override;

    virtual uint32_t width() const override;

    virtual uint32_t height() const override;

    virtual uint32_t refresh_rate() const override;

    bool needs_refresh() const { return needs_refresh_; }

    void set_needs_refresh(bool value) { needs_refresh_ = value; }

protected:
    // Called by DisplayLinkWin whenever the physical display is refreshed
    void OnDisplayRefresh();

    uint32_t display_id_ = 0;
    WindowsUtil* util_ = nullptr;
    HMONITOR monitor_ = nullptr;
    std::atomic<bool> needs_refresh_ = false;
    std::unique_ptr<DisplayLinkWin> display_link_;
};

} // namespace ultralight
