#include "voxel_model.h"
#include "engine.h"
#include "fstream"
#define OGT_VOX_IMPLEMENTATION
#include "ogt_vox.h"
namespace arc {
void VoxelModel::Setup(const std::string &file_path) {
  if (setup_)
    Dispose();
  file_path_ = Engine::config().asset_folder + file_path;
  logi("{0}", file_path_);

  std::ifstream file(file_path_);
  if (file.is_open()) {
    logi("Open");
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    uint8_t *buffer = new uint8_t[size];
    file.read(reinterpret_cast<char *>(buffer), size);
    const ogt_vox_scene *scene = ogt_vox_read_scene(buffer, size);
    delete[] buffer;
    logi("Groups: {0}", scene->num_groups);
    logi("Instances: {0}", scene->num_instances);
    logi("Layers: {0}", scene->num_layers);
    logi("Models: {0}", scene->num_models);

    bool added[256]{false};
    added[0] = true;

    textures_ = new Texture[scene->num_instances];
    voxel_datas_ = new uint8_t *[scene->num_instances];
    shadow_masks_ = new uint8_t *[scene->num_instances];
    active_shadow_masks_ = new uint8_t *[scene->num_instances];
    sizes_ = new glm::vec3[scene->num_instances];
    raw_sizes_ = new glm::vec3[scene->num_instances];
    offsets_ = new glm::vec3[scene->num_instances];
    shadow_bounds_ = new Bounds[scene->num_instances];
    for (int inst = 0; inst < scene->num_instances; ++inst) {
      auto model = scene->models[scene->instances[inst].model_index];
      logi("x: {0}", model->size_x);
      logi("y: {0}", model->size_y);
      logi("z: {0}", model->size_z);
      size_t x_size = model->size_x;
      size_t y_size = model->size_y;
      size_t z_size = model->size_z;
      offsets_[inst] = (*((glm::mat4 *)&scene->instances[inst].transform))[3];
      for (int i = 7; i >= 2; --i) {
        if (x_size & (1 << i)) {
          if ((x_size & ((1 << i) - 1)))
            x_size = 1 << (i + 1);
          break;
        } else if (i == 2) {
          x_size = 4;
        }
      }
      for (int i = 7; i >= 0; --i) {
        if (y_size & (1 << i)) {
          if ((y_size & ((1 << i) - 1)))
            y_size = 1 << (i + 1);
          break;
        } else if (i == 2) {
          y_size = 4;
        }
      }

      // has to be
      for (int i = 7; i >= 0; --i) {
        if (z_size & (1 << i)) {
          if ((z_size & ((1 << i) - 1)))
            z_size = 1 << (i + 1);
          break;
        } else if (i == 2) {
          z_size = 4;
        }
      }
      offsets_[inst] += glm::vec3(x_size, y_size, z_size) / 2.0f;
      offsets_[inst] -=
          glm::vec3(model->size_x, model->size_y, model->size_z) / 2.0f;
      sizes_[inst] = {x_size, y_size, z_size};
      raw_sizes_[inst] = sizes_[inst];
      offsets_[inst] /= sizes_[inst];
      logi("fixed x: {0}", x_size);
      logi("fixed y: {0}", y_size);
      logi("fixed z: {0}", z_size);
      size_t size1 = x_size * y_size * z_size;
      logi("size {0}", size1);
      voxel_datas_[inst] = new uint8_t[size1]{0};
      shadow_masks_[inst] = new uint8_t[size1 >> 3];
      active_shadow_masks_[inst] = new uint8_t[size1 ];// 2 times larger in every direction
      for (int i = 0; i < size1 >> 3; ++i) {
        shadow_masks_[inst][i] = 0;
      }
      for (int x = 0; x < model->size_x; ++x) {
        for (int y = 0; y < model->size_y; ++y) {
          for (int z = 0; z < model->size_z; ++z) {
            int i_in =
                z * model->size_x * model->size_y + y * model->size_x + x;
            int i_out = z * x_size * y_size + y * x_size + x;
            voxel_datas_[inst][i_out] = model->voxel_data[i_in];
          }
        }
      }

      for (int y = 0; y < model->size_y; ++y) {
        for (int x = 0; x < model->size_x; ++x) {
          for (int z = 0; z < model->size_z; ++z) {
            int i = x * z_size * y_size + y * z_size + z;
            int is =
                (x / 2) * z_size * y_size / 4 + (y / 2) * z_size / 2 + z / 2;
            int ib = 1 << ((x & 1) * 4 + (y & 1) * 2 + (z & 1));
            uint8_t color = voxel_datas_[inst][i];
            /* printf("%d", (bool)color); */
            if (color) {
              shadow_masks_[inst][is] |= ib;
            }

            if (!added[color]) {
              added[color] = true;
              materials_.push_back({{scene->palette.color[color].r / 255.0f,
                                     scene->palette.color[color].g / 255.0f,
                                     scene->palette.color[color].b / 255.0f}});
              material_lookup_[color] = materials_.size();
            }
            voxel_datas_[inst][i] = material_lookup_[color];
          }
          /* printf("\n"); */
        }
        /* printf("\n"); */
        /* printf("\n"); */
      }

      TexConf config = {TexConf::R, TexConf::UNSIGNED_BYTE, TexConf::NEAREST};
      textures_[inst].Setup3D(x_size, y_size, z_size, voxel_datas_[inst],
                              config);
    }
    ogt_vox_destroy_scene(scene);
    file.close();
    setup_ = true;
  } else {
    loge("Couldn't open model");
  }
}
void VoxelModel::Dispose() {
  if (!setup_)
    return;
  setup_ = false;
  for (int i = 0; i < num_instances_; ++i) {
    delete[] voxel_datas_[i];
    delete[] shadow_masks_[i];
    delete[] active_shadow_masks_[i];
  }
  delete[] voxel_datas_;
  delete[] shadow_masks_;
  delete[] active_shadow_masks_;
  delete[] sizes_;
  delete[] raw_sizes_;
  delete[] offsets_;
  delete[] textures_;
}
} // namespace arc
