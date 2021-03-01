///
/// @file Monitor.h
///
/// @brief The header for the Monitor class.
///
/// @author
///
/// This file is a part of Ultralight, a next-generation HTML renderer.
///
/// Website: <http://ultralig.ht>
///
/// Copyright (C) 2021 Ultralight, Inc. All rights reserved.
///
#pragma once
#include "Defines.h"

namespace ultralight {

///
/// Monitor class, represents a platform monitor.
///
class AExport Monitor {
public:
  virtual ~Monitor() {}

  ///
  /// Get the DPI scale (1.0 = 100%)
  ///
  virtual double scale() const = 0;

  ///
  /// Get the width of the monitor.
  ///
  virtual uint32_t width() const = 0;

  /// Get the height of the monitor.
  ///
  virtual uint32_t height() const = 0;
};

}  // namespace ultralight
