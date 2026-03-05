#include "ThreadCPUMonitor.h"
#include <algorithm>
#include <cassert>

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#elif defined(__APPLE__) || defined(__linux__)
#  include <time.h>
#else
#  error "Unsupported platform"
#endif

namespace ultralight {

ThreadCPUMonitor::ThreadCPUMonitor(double window_seconds) {
  assert(window_seconds >= 0.1);
  window_ns_ = static_cast<uint64_t>(window_seconds * 1e9);
}

void ThreadCPUMonitor::Sample() {
  ring_[head_] = { WallNs(), ThreadCpuNs() };
  head_ = (head_ + 1) % kRingCapacity;
  if (count_ < kRingCapacity)
    count_++;
}

double ThreadCPUMonitor::GetThreadUtilization() const {
  if (count_ < 2)
    return 1.0;

  size_t newest_idx = (head_ + kRingCapacity - 1) % kRingCapacity;
  const auto& newest = ring_[newest_idx];

  // Walk backward through the ring buffer to find the oldest sample
  // that's at least window_ns_ in the past.
  size_t best_idx = (newest_idx + kRingCapacity - 1) % kRingCapacity;
  for (size_t i = 2; i < count_; i++) {
    size_t idx = (newest_idx + kRingCapacity - i) % kRingCapacity;
    best_idx = idx;
    if (newest.wall_ns - ring_[idx].wall_ns >= window_ns_)
      break;
  }

  const auto& oldest = ring_[best_idx];
  uint64_t wall_delta = newest.wall_ns - oldest.wall_ns;
  if (wall_delta == 0)
    return 1.0;

  uint64_t cpu_delta = newest.cpu_ns - oldest.cpu_ns;
  double util = static_cast<double>(cpu_delta) / static_cast<double>(wall_delta);
  return std::min(util, 1.0);
}

// ── Platform: Windows ────────────────────────────────────────────────

#if defined(_WIN32)

uint64_t ThreadCPUMonitor::WallNs() {
  // Thread-safe static init (C++11 magic statics)
  static const uint64_t freq = [] {
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    return static_cast<uint64_t>(f.QuadPart);
  }();

  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);

  // Convert to nanoseconds avoiding 64-bit overflow
  uint64_t ticks = static_cast<uint64_t>(now.QuadPart);
  uint64_t sec = ticks / freq;
  uint64_t rem = ticks % freq;
  return sec * 1'000'000'000ULL + rem * 1'000'000'000ULL / freq;
}

uint64_t ThreadCPUMonitor::ThreadCpuNs() {
  FILETIME creation, exit, kernel, user;
  GetThreadTimes(GetCurrentThread(), &creation, &exit, &kernel, &user);

  // FILETIME is in 100-nanosecond intervals
  auto to_ns = [](const FILETIME& ft) -> uint64_t {
    uint64_t t = (static_cast<uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
    return t * 100;
  };
  return to_ns(kernel) + to_ns(user);
}

// ── Platform: macOS / Linux ──────────────────────────────────────────

#else

uint64_t ThreadCPUMonitor::WallNs() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return static_cast<uint64_t>(ts.tv_sec) * 1'000'000'000ULL +
         static_cast<uint64_t>(ts.tv_nsec);
}

uint64_t ThreadCPUMonitor::ThreadCpuNs() {
  struct timespec ts;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
  return static_cast<uint64_t>(ts.tv_sec) * 1'000'000'000ULL +
         static_cast<uint64_t>(ts.tv_nsec);
}

#endif

} // namespace ultralight
