#pragma once
#include <cstddef>
#include <cstdint>

namespace ultralight {

/// Measures per-thread CPU utilization using OS-level APIs.
///
/// Call Sample() once per frame from the thread you want to monitor.
/// GetThreadUtilization() returns a smoothed utilization ratio (0.0-1.0)
/// over a configurable time window.
///
/// Platform APIs:
///   Windows:     QueryPerformanceCounter() + GetThreadTimes()
///   macOS/Linux: clock_gettime(CLOCK_MONOTONIC) + clock_gettime(CLOCK_THREAD_CPUTIME_ID)
class ThreadCPUMonitor {
public:
  /// @param window_seconds  Averaging window duration (default: 1.0s).
  ///   Longer = more stable, shorter = more responsive.
  ///   On Windows, >= 0.5s recommended due to GetThreadTimes() ~15.6ms resolution.
  explicit ThreadCPUMonitor(double window_seconds = 1.0);

  /// Sample the calling thread's CPU time and wall-clock time.
  /// Must be called from the same thread every time.
  void Sample();

  /// Returns CPU utilization of the calling thread (0.0-1.0) over
  /// the configured window. Returns 1.0 if insufficient data.
  double GetThreadUtilization() const;

private:
  struct FrameSample {
    uint64_t wall_ns;
    uint64_t cpu_ns;
  };

  static constexpr size_t kRingCapacity = 512;

  FrameSample ring_[kRingCapacity];
  size_t head_ = 0;
  size_t count_ = 0;
  uint64_t window_ns_;

  static uint64_t WallNs();
  static uint64_t ThreadCpuNs();
};

} // namespace ultralight
