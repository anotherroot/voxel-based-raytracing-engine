#ifndef ARC_ENGINE_FRAME_BUFFER_H
#define ARC_ENGINE_FRAME_BUFFER_H
#include "structs.h"
namespace arc{
  class FrameBuffer{
    public:
      void Setup(const FrameBufferConfig& config);
      void Dispose();
      ~FrameBuffer(){Dispose();}
      operator bool() const {return id_;}
      void Bind();
      void Unbind();
      void Resize(uint width, uint height);
      uint GetAttachment(uint index) const {return attachments_[index];}
      void Bind(uint index, int slot);
    private:
      void Update();

      uint width_, height_, samples_;
      bool depth_stencil_;
      uint attachments_[33];
      uint draw_buffers_[32];
      uint id_{0};
      uint size_;
      uint internal_formats_[32];
      uint data_formats_[32];
      uint internal_size_formats_[32];
      uint interpolations_[32];

  };
}
#endif // ARC_ENGINE_FRAME_BUFFER_H
