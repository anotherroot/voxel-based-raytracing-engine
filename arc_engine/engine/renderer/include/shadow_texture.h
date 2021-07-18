#ifndef ARC_ENGINE_SHADOW_TEXTURE_H
#define ARC_ENGINE_SHADOW_TEXTURE_H
#include "core.h"
#include "glm.hpp"
#include "texture.h"
#include "voxel_model.h"
namespace arc{
  class ShadowTexture{
    public:
      void Init(uint width, uint height, uint depth, const glm::vec3 &size, const glm::vec3 &pos);
      void Destroy();
      const Texture& texture() const {return texture_;}
      const glm::vec3 &size() const {return size_;}
      void SetBits(const VoxelModel& model, const glm::vec3 &position);// TODO: add rotation and scale
    private:
      uint8_t *data_;
      Texture texture_;
      uint width_, height_, depth_;
      glm::vec3 size_, pos_;
  };
}

#endif // ARC_ENGINE_SHADOW_TEXTURE_H
