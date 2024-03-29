#pragma once
#include <AppCore/Monitor.h>
#import "Cocoa/Cocoa.h"

namespace ultralight {
    
class MonitorMac : public Monitor {
public:
  MonitorMac();
  virtual ~MonitorMac() {}

  virtual uint32_t display_id() const override { return display_id_; }

  virtual double scale() const override;

  virtual uint32_t width() const override;

  virtual uint32_t height() const override;

  virtual uint32_t refresh_rate() const override;

protected:
  uint32_t display_id_ = 0;
  NSScreen* screen_ = nullptr;
};

}  // namespace ultralight

