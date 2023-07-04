/******************************************************************************
 *  This file is a part of Ultralight, an ultra-portable web-browser engine.  *
 *                                                                            *
 *  See <https://ultralig.ht> for licensing and more.                         *
 *                                                                            *
 *  (C) 2023 Ultralight, Inc.                                                 *
 *****************************************************************************/
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
