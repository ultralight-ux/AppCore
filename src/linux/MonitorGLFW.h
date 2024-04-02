#pragma once
#include <AppCore/Monitor.h>

typedef struct GLFWmonitor GLFWmonitor;

namespace ultralight {

class MonitorGLFW : public Monitor {
public:
  MonitorGLFW();
  virtual ~MonitorGLFW() {}

  virtual uint32_t display_id() const { return display_id_; }

  virtual double scale() const;

  virtual uint32_t width() const;

  virtual uint32_t height() const;

  virtual uint32_t refresh_rate() const;

protected:
  GLFWmonitor* monitor_;
  uint32_t display_id_;
};

}  // namespace ultralight
