#pragma once
#include <AppCore/App.h>
#include "ThreadCPUMonitor.h"
#include "FileLogger.h"
#include <chrono>
#include <memory>

namespace ultralight {

///
/// Internal base class for platform App implementations.
///
/// Centralizes shared state (settings, listener, renderer, logger) and idle
/// detection logic that would otherwise be duplicated across AppWin, AppMac,
/// and AppGLFW.
///
/// Platform subclasses inherit from this instead of App directly.
///
class AppImpl : public App {
public:
  // --- Trivial overrides (identical across all platforms) ---

  const Settings& settings() const override { return settings_; }

  void set_listener(AppListener* listener) override { listener_ = listener; }

  AppListener* listener() override { return listener_; }

  bool is_running() const override { return is_running_; }

  RefPtr<Renderer> renderer() override { return renderer_; }

  // --- Idle getters ---

  bool is_idle() const override;

  double thread_utilization() const override;

  // --- Input tracking (called by Window Fire*Event methods) ---

  void NotifyUserInteraction();

protected:
  AppImpl() = default;
  virtual ~AppImpl() = default;

  /// Call at the beginning of each platform's Update() method.
  /// Dispatches listener_->OnUpdate() and renderer()->Update().
  void UpdateBegin();

  /// Call at the end of each platform's Update() method.
  /// Samples CPU utilization, runs the idle state machine, and fires
  /// renderer()->Recycle() + listener_->OnIdle() when appropriate.
  void UpdateIdleDetection();

  // --- Shared members (moved from all 3 platform classes) ---

  Settings settings_;
  bool is_running_ = false;
  AppListener* listener_ = nullptr;
  RefPtr<Renderer> renderer_;
  std::unique_ptr<FileLogger> logger_;
  std::chrono::steady_clock::time_point last_statistics_update_;

  // --- Idle detection state ---

  ThreadCPUMonitor cpu_monitor_{1.0};

  enum class IdleState { Active, Pending, Cooldown };
  IdleState idle_state_ = IdleState::Active;

  std::chrono::steady_clock::time_point idle_pending_start_;
  std::chrono::steady_clock::time_point last_idle_fire_;
  std::chrono::steady_clock::time_point last_update_time_{std::chrono::steady_clock::now()};
  std::chrono::steady_clock::time_point last_user_input_time_{std::chrono::steady_clock::now()};
};

} // namespace ultralight
