#pragma once
#if defined(_WIN32)

#include <Ultralight/platform/ThreadManager.h>
#include <mutex>
#include <string>
#include <map>

using namespace ultralight;

class ThreadManagerWin : public ThreadManager {
 public:
  ThreadManagerWin();
  virtual ~ThreadManagerWin();

  virtual bool CreateThread(const char* name, ThreadQOS qos, ThreadEntryPoint entry_point,
                            void* entry_point_data, CreateThreadResult& result) override;

  void OnThreadExit(uint32_t creation_id);

 protected:
  struct ThreadInfo {
    std::string name;
  };

  std::map<uint32_t, ThreadInfo> thread_map_;
  std::mutex thread_map_lock_;
  uint32_t next_creation_id_ = 0;
};

#endif // defined(_WIN32)