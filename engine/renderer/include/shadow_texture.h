#ifndef ARC_ENGINE_SHADOW_TEXTURE_H
#define ARC_ENGINE_SHADOW_TEXTURE_H
#include "core.h"
#include "glm.hpp"
#include "texture.h"
#include "voxel_model.h"
namespace arc {
class ShadowTexture {

public:
  void Init(uint width, uint height, uint depth, const glm::vec3 &size,
            const glm::vec3 &pos);
  void Destroy();
  const Texture &texture() const { return texture_; }
  const glm::vec3 &size() const { return size_; }
  void SetBits(const VoxelModel &model, const glm::mat4 &transform,
               bool set = true); // TODO: add rotation and scale
  inline void Bind(int slot) {
    if (bounds_.set) {
      texture_.set_data(data_, bounds_.min_x, bounds_.min_y, bounds_.min_z, bounds_.max_x - bounds_.min_x + 1,bounds_.max_y - bounds_.min_y + 1,bounds_.max_z - bounds_.min_z + 1);

      /* uint8_t data[512]; */
      /* for (int i = 0; i < 512; ++i) { */
      /*   if(i%8==0) */
      /*   data[i] = 255; */ 
      /* } */
      /* logi("set"); */
      /* texture_.set_data(data, 1*width_ / 4, 1*height_/4,1*depth_/4, 8, 8, 8); */

      ResetBounds();
    }
    texture_.Bind(slot);
  }

private:
  uint8_t *data_;
  Texture texture_;
  uint width_, height_, depth_;
  glm::vec3 size_, pos_;

  struct Bounds {
    bool set{false};
    int min_x, max_x;
    int min_y, max_y;
    int min_z, max_z;
  };
  inline void ResetBounds() {
    bounds_.set = false;
    bounds_.min_x = INT32_MAX;
    bounds_.max_x = 0;
    bounds_.min_y = INT32_MAX;
    bounds_.max_y = 0;
    bounds_.min_z = INT32_MAX;
    bounds_.max_z = 0;
  }
  Bounds bounds_;
};
} // namespace arc

#endif // ARC_ENGINE_SHADOW_TEXTURE_H
