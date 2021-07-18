#include "frame_buffer.h"
#include "glad/glad.h"
#include "opengl_func.h"
namespace arc {

void FrameBuffer::Setup(const FrameBufferConfig &config) {
  width_ = config.width;
  height_ = config.height;
  samples_ = config.samples;
  depth_stencil_ = config.depth_stencil;
  render_id_ = 0;
  for (int i = 0; i < 32; ++i) {
    draw_buffers_[i] = GL_COLOR_ATTACHMENT0 + i;
  }
  size_ = 0;
  for (auto &frame : config.frames) {
    internal_formats_[size_] = GetInternalFormat(frame);
    data_formats_[size_] = GetDataFormat(frame);
    internal_size_formats_[size_] = GetSizedInternalFormat(frame);
    interpolations_[size_] = GetInterpolation(frame);
    size_++;
  }

  Update();
}
void FrameBuffer::Dispose() {
  if (render_id_) {
    glDeleteFramebuffers(1, &render_id_);
    glDeleteTextures(size_ + depth_stencil_, attachments_);
  }
}
void FrameBuffer::Bind() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, render_id_);
  glViewport(0,0, width_, height_);
}
void FrameBuffer::Unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::Resize(uint width, uint height) {
  if(width >0 && height >0){
    width_ = width; height_ = height;
    Update();
  }
}
void FrameBuffer::Update() {
  Dispose();

  glCreateFramebuffers(1, &render_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, render_id_);
  glDrawBuffers(size_, (GLenum *)draw_buffers_);
  bool multisample = samples_ > 1;

  glCreateTextures(GL_TEXTURE_2D, size_ + depth_stencil_, attachments_);
  for (int i = 0; i < size_; ++i) {

    glBindTexture(GL_TEXTURE_2D, attachments_[i]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolations_[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolations_[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_size_formats_[i], width_, height_,
                 0, internal_formats_[i], data_formats_[i], nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, draw_buffers_[i], GL_TEXTURE_2D,
                           attachments_[i], 0);
  }
  if (depth_stencil_) {
    glBindTexture(GL_TEXTURE_2D, attachments_[size_]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width_, height_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D, attachments_[size_], 0);
  }
  arc_assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) ==GL_FRAMEBUFFER_COMPLETE, "Framebuffer setup failsed!");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::Bind(uint index, int slot){
  glBindTextureUnit(slot,attachments_[index]);
}
} // namespace arc

