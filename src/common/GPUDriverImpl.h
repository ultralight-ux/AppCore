#pragma once
#include <AppCore/Defines.h>
#include <Ultralight/platform/GPUDriver.h>
#include <vector>

namespace ultralight {

class GPUDriverImpl : public GPUDriver {
public:
  GPUDriverImpl();
  virtual ~GPUDriverImpl();

  virtual const char* name() = 0;

  virtual void BeginDrawing() = 0;

  virtual void EndDrawing() = 0;

  virtual void BindTexture(uint8_t texture_unit, uint32_t texture_id) = 0;

  virtual void BindRenderBuffer(uint32_t render_buffer_id) = 0;

  virtual void ClearRenderBuffer(uint32_t render_buffer_id) = 0;

  virtual void DrawGeometry(uint32_t geometry_id,
                            uint32_t indices_count,
                            uint32_t indices_offset,
                            const GPUState& state) = 0;

  virtual bool HasCommandsPending();

  virtual void DrawCommandList();

  virtual int batch_count() const;

  // Inherited from GPUDriver

  virtual void BeginSynchronize() override;

  virtual void EndSynchronize() override;

  virtual uint32_t NextTextureId() override;

  virtual uint32_t NextRenderBufferId() override;

  virtual uint32_t NextGeometryId() override;

  virtual void UpdateCommandList(const CommandList& list) override;

protected:
  uint32_t next_texture_id_ = 1;
  uint32_t next_render_buffer_id_ = 1; // render buffer id 0 is reserved for default render target view.
  uint32_t next_geometry_id_ = 1;
  std::vector<Command> command_list_;
  int batch_count_;
};

}  // namespace ultralight
