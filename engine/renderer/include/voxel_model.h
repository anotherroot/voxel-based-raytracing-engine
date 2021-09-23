#ifndef ARC_ENGINE_VOXEL_MODEL_H
#define ARC_ENGINE_VOXEL_MODEL_H
#include "voxel_renderer.h"
#include "components.h"
#include "unordered_map"
#include "string"
namespace arc {
class VoxelModels {

public:
  struct ModelData{
    ModelComponent comp;
    uint8_t *data;
    uint8_t *sdf;
    Texture texture;
    std::string file_path;
  };
  struct MaterialData{
    Material mat;
    uint64_t code;
    int index;
  };
  const ModelComponent& AddModel(const std::string& name, int width, int height, int depth, uint8_t* data, const std::vector<Material>& materials);
  const ModelComponent& AddModel(const std::string& file_path);
  void Dispose();
  ModelData *GetModel(const std::string& name) const{
    arc_assert(models_.find(name) == models_.end(), "model with name {0} doesnt exist!", name);
    return models_.find(name)->second;
  }

  const std::unordered_map<std::string, ModelData*> &models() const { return models_; }
  const std::unordered_map<uint64_t, MaterialData> &materials() const { return materials_; }

private:

  std::unordered_map<std::string, ModelData*> models_;
  std::unordered_map<uint64_t, MaterialData> materials_;

};
} // namespace arc
#endif // ARC_ENGINE_VOXEL_MODEL_H
