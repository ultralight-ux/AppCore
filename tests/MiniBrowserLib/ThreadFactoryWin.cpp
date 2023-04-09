#include "ThreadFactoryWin.h"

#if defined(_WIN32)
#include <Windows.h>
#include <process.h>

ThreadFactoryWin* g_instance = nullptr;

struct ThreadContext {
  ultralight::ThreadEntryPoint entry_point;
  void* entry_point_data;
  uint32_t creation_id;
};

static unsigned __stdcall ultralightThreadEntryPoint(void* ctx) {
  ThreadContext* context = reinterpret_cast<ThreadContext*>(ctx);
  (*context->entry_point)(context->entry_point_data);

  if (g_instance)
    g_instance->OnThreadExit(context->creation_id);

  delete context;

  return 0;
}

ThreadFactoryWin::ThreadFactoryWin() { g_instance = this; }

ThreadFactoryWin::~ThreadFactoryWin() {

  std::lock_guard<std::mutex> lock(thread_map_lock_);
  if (thread_map_.size()) {
    printf("Leaked threads: \n");
    for (auto i = thread_map_.begin(); i != thread_map_.end(); i++) {
      printf("\tThread: %d, %s\n", i->first, i->second.name.c_str());
    }
    MessageBoxA(NULL, "Leaked threads detected, check console.", "Caption", MB_OKCANCEL);
  }

  g_instance = nullptr;
}

bool ThreadFactoryWin::CreateThread(const char* name, ThreadType type, ThreadEntryPoint entry_point,
                                    void* entry_point_data, CreateThreadResult& result) {

  uint32_t creation_id = next_creation_id_++;

  ThreadContext* context = new ThreadContext;
  context->entry_point = entry_point;
  context->entry_point_data = entry_point_data;
  context->creation_id = creation_id;

  result.handle = (ThreadHandle) reinterpret_cast<HANDLE>(
      _beginthreadex(0, 0, ultralightThreadEntryPoint, context, 0, &result.id));

  ThreadInfo data { name };
  {
    std::lock_guard<std::mutex> lock(thread_map_lock_);
    thread_map_.insert({ creation_id, data });
  }

  printf("New thread: %d, %s\n", creation_id, name);

  return true;
}

void ThreadFactoryWin::OnThreadExit(uint32_t creation_id) {
  std::lock_guard<std::mutex> lock(thread_map_lock_);
  auto i = thread_map_.find(creation_id);
  if (i != thread_map_.end()) {
    printf("Thread exit: %d, %s\n", i->first, i->second.name.c_str());
    thread_map_.erase(i);
  }
}

#endif
