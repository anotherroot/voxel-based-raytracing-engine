#ifndef ARC_ENGINE_VOXEL_MODEL_H
#define ARC_ENGINE_VOXEL_MODEL_H
#include "voxel_renderer.h"
namespace arc {
class VoxelModel {
public:
  void Setup(const std::string &file_path);
  void Dispose();

  const Texture &texture(int inst) const { return textures_[inst]; }
  uint8_t * data(int inst) const { return voxel_datas_[inst]; }
  uint8_t * shadow_mask(int inst) const { return shadow_masks_[inst]; }
  int num_instances() const { return num_instances_; }
  const std::vector<Material> &materials() const { return materials_; }
  const glm::vec3 &size(int inst) const {return sizes_[inst];}

private:
  bool setup_{false};
  std::string file_path_;

  int num_instances_;
  glm::vec3 *sizes_;
  glm::vec3 *offsets_;
  uint8_t **voxel_datas_;
  uint8_t **shadow_masks_;
  std::vector<Material> materials_;
  Texture *textures_;
  int material_lookup_[256]{0};
};
} // namespace arc
#endif // ARC_ENGINE_VOXEL_MODEL_H
