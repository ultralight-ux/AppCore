#include "AppImpl.h"
#include <Ultralight/Renderer.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>
#include <Ultralight/private/tracy/Tracy.hpp>

namespace ultralight {

bool AppImpl::is_idle() const {
  return idle_state_ != IdleState::Active;
}

double AppImpl::thread_utilization() const {
  return cpu_monitor_.GetThreadUtilization();
}

void AppImpl::NotifyUserInteraction() {
  last_user_input_time_ = std::chrono::steady_clock::now();
}

void AppImpl::UpdateBegin() {
  if (listener_)
    listener_->OnUpdate();

#ifdef TRACY_PROFILE_PERFORMANCE
  const char* frame_mark_update = "Update";
  FrameMarkStart(frame_mark_update);
#endif

  renderer()->Update();

#ifdef TRACY_PROFILE_PERFORMANCE
  FrameMarkEnd(frame_mark_update);
#endif
}

void AppImpl::UpdateIdleDetection() {
  auto now = std::chrono::steady_clock::now();

  // Staleness guard: if gap between Update() calls exceeds 100ms, the app was
  // likely frozen or had a severe stall. Reset idle state since stale utilization
  // data from ThreadCPUMonitor is not trustworthy.
  constexpr auto kStalenessThreshold = std::chrono::milliseconds(100);
  bool stale = (now - last_update_time_) > kStalenessThreshold;
  last_update_time_ = now;

  cpu_monitor_.Sample();
  double utilization = cpu_monitor_.GetThreadUtilization();
  double secs_since_input =
      std::chrono::duration<double>(now - last_user_input_time_).count();

  bool idle_conditions = !stale
                      && secs_since_input > settings_.idle_threshold
                      && utilization < settings_.idle_utilization_threshold;

  if (!idle_conditions) {
    idle_state_ = IdleState::Active;
  } else {
    switch (idle_state_) {
    case IdleState::Active:
      idle_state_ = IdleState::Pending;
      idle_pending_start_ = now;
      break;

    case IdleState::Pending: {
      double sustained =
          std::chrono::duration<double>(now - idle_pending_start_).count();
      if (sustained >= settings_.sustained_idle_time) {
        renderer()->Recycle();
        if (listener_)
          listener_->OnIdle(utilization);
        last_idle_fire_ = now;
        idle_state_ = IdleState::Cooldown;
      }
      break;
    }

    case IdleState::Cooldown: {
      double since_fire =
          std::chrono::duration<double>(now - last_idle_fire_).count();
      if (since_fire >= settings_.sustained_idle_time) {
        renderer()->Recycle();
        if (listener_)
          listener_->OnIdle(utilization);
        last_idle_fire_ = now;
        // Stay in Cooldown, will fire again after next interval
      }
      break;
    }
    }
  }
}

} // namespace ultralight
