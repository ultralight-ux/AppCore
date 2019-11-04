#if defined(DRIVER_D3D12)
#pragma once
#include <d3d12.h>
#include <stdint.h>
#include <vector>
#include <map>
#include <stack>
#include <memory>
#include <wrl/client.h>
#include "d3dx12.h"

class DescriptorPool;

//
// Smart pointer for managing descriptor handle allocations. No reference counting is performed,
// every DescriptorHandle maintains an owning reference similar to std::unique_ptr<>.
//
// The handle is returned to the allocator's pool when it is destroyed.
//
class DescriptorHandle {
public:
  // null allocation
  DescriptorHandle();

  DescriptorHandle(DescriptorPool* pool, ID3D12DescriptorHeap* heap, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle);

  // Move constructor (copy is not allowed)
  DescriptorHandle(DescriptorHandle&& other);

  // Move assignment (assignment is not allowed)
  DescriptorHandle& operator=(DescriptorHandle&& other);

  ~DescriptorHandle();

  DescriptorPool* descriptor_pool() const { return descriptor_pool_; }

  ID3D12DescriptorHeap* descriptor_heap() const { return descriptor_heap_; }

  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle() const { return cpu_handle_; }

protected:
  // No copies, only move allowed
  DescriptorHandle(const DescriptorHandle&) = delete;
  DescriptorHandle& operator= (const DescriptorHandle&) = delete;

  DescriptorPool* descriptor_pool_;
  ID3D12DescriptorHeap* descriptor_heap_;
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
};

//
// Free-list allocator for descriptor handles of a certain heap type. No memory is released to system
// in this implementation.
//
class DescriptorPool {
public:
  DescriptorPool(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type);

  DescriptorHandle Create();

  D3D12_DESCRIPTOR_HEAP_TYPE type() const { return type_; }

protected:
  ID3D12DescriptorHeap* CreateNewHeap();

  void Free(DescriptorHandle* handle);

  ID3D12Device* device_;
  D3D12_DESCRIPTOR_HEAP_TYPE type_;
  static const uint32_t num_descriptors_per_heap_ = 256;
  size_t descriptor_size_ = 0;
  size_t remaining_free_handles_ = 0;
  ID3D12DescriptorHeap* current_heap_ = nullptr;
  D3D12_CPU_DESCRIPTOR_HANDLE current_handle_ = { 0 };
  std::vector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> active_heaps_;
  std::stack<DescriptorHandle> free_list_;
  friend class DescriptorHandle;
};

//
// Descriptor allocator interface. Provides a front-end for DescriptorPool.
//
class DescriptorAllocator {
public:
  DescriptorAllocator(ID3D12Device* device);

  // Create a single descriptor for a certain descriptor heap type. Returns a smart pointer.
  DescriptorHandle Create(D3D12_DESCRIPTOR_HEAP_TYPE type);

protected:
  std::unique_ptr<DescriptorPool> pool_by_type_[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
};

//
// Fixed-size ring buffer for passing descriptors to GPU.
//
class GPUDescriptorRingBuffer {
public:
  GPUDescriptorRingBuffer(ID3D12Device* device);

  D3D12_GPU_DESCRIPTOR_HANDLE StoreTable(DescriptorHandle* cbv0, DescriptorHandle* srv0, DescriptorHandle* srv1);

  ID3D12DescriptorHeap* descriptor_heap() const { return cbv_srv_uav_gpu_descriptor_heap_.Get(); }

protected:
  ID3D12Device* device_;
  static const uint32_t num_descriptors_ = 16384 * 4;
  Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cbv_srv_uav_gpu_descriptor_heap_;
  CD3DX12_CPU_DESCRIPTOR_HANDLE current_cpu_handle_;
  CD3DX12_GPU_DESCRIPTOR_HANDLE current_gpu_handle_;
  INT descriptor_size_ = 0;
  size_t remaining_handles_ = 0;
};

#endif // if defined(DRIVER_D3D12)
