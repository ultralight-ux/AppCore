#include "GPUDriverImpl.h"

namespace ultralight {

GPUDriverImpl::GPUDriverImpl() : batch_count_(0) {}

GPUDriverImpl::~GPUDriverImpl() {}

bool GPUDriverImpl::HasCommandsPending() {
  return !command_list_.empty();
}

void GPUDriverImpl::DrawCommandList() {
  if (command_list_.empty())
    return;

  batch_count_ = 0;

  for (auto& cmd : command_list_) {
    if (cmd.command_type == CommandType::DrawGeometry)
      DrawGeometry(cmd.geometry_id, cmd.indices_count, cmd.indices_offset, cmd.gpu_state);
    else if (cmd.command_type == CommandType::ClearRenderBuffer)
      ClearRenderBuffer(cmd.gpu_state.render_buffer_id);
    batch_count_++;
  }

  command_list_.clear();
}

int GPUDriverImpl::batch_count() const {
  return batch_count_;
}

void GPUDriverImpl::BeginSynchronize() {}

void GPUDriverImpl::EndSynchronize() {}

uint32_t GPUDriverImpl::NextTextureId() { return next_texture_id_++; }

uint32_t GPUDriverImpl::NextRenderBufferId() { return next_render_buffer_id_++; }

uint32_t GPUDriverImpl::NextGeometryId() { return next_geometry_id_++; }

void GPUDriverImpl::UpdateCommandList(const CommandList& list) {
  if (list.size) {
    command_list_.resize(list.size);
    memcpy(&command_list_[0], list.commands, sizeof(Command) * list.size);
  }
}

}  // namespace ultralight
