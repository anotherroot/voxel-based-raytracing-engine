#include "voxel_model.h"
#include "engine.h"
#include "fstream"
#define OGT_VOX_IMPLEMENTATION
#include "ogt_vox.h"
namespace arc {
  uint64_t getMatIndex(const glm::vec3 &color, float refraction, float reflection, float roughness){
    return ((uint64_t)(refraction*255 )<< 40)+((uint64_t)(reflection*255 )<< 32)+((uint64_t)(roughness*255 )<< 24)+((uint64_t)(color.r*255) << 16)+((uint64_t)(color.r*255 )<< 8)+((uint64_t)(color.r*255) );
  }
const ModelComponent &
VoxelModels::AddModel(const std::string &name, int width, int height, int depth,
                      uint8_t *data, const std::vector<Material> &materials) {
  ModelData *model = new ModelData();

  model->file_path = name;

  glm::vec3 raw_size, size, offset;
  size_t x_size, y_size, z_size;
  { // get size
    x_size = width;
    y_size = height;
    z_size = depth;

    raw_size = {x_size, y_size, z_size};
    offset = {0, 0, 0};
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
    size = {x_size, y_size, z_size};
    logi("fixed x: {0}", x_size);
    logi("fixed y: {0}", y_size);
    logi("fixed z: {0}", z_size);
  }
  size_t full_size = x_size * y_size * z_size;
  model->data = new uint8_t[full_size * 3];
  for (int i = 0; i < full_size * 3; ++i) {
    model->data[i] = 0;
  }
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      for (int z = 0; z < depth; ++z) {
        int i_in = z * width * height + y * width + x;
        int i_out = (z * x_size * y_size + y * x_size + x) * 3;
        model->data[i_out] = data[i_in];
      }
    }
  }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      for (int z = 0; z < depth; ++z) {
        int i = (z * x_size * y_size + y * x_size + x) * 3;
        /* int i = x * z_size * y_size + y * z_size + z; */
        uint8_t ci = model->data[i];

        if (ci) {
          auto mat = materials[ci - 1];

          uint64_t index = getMatIndex(mat.color, mat.refraction, mat.reflection, mat.roughness);
          auto mat_p = materials_.find(index);
          if (mat_p == materials_.end()) {
            MaterialData data;
            data.mat = mat;
            data.index = materials_.size() + 1;
            arc_assert(data.index < 256,
                       "Too many diferent materials in scene!");
            data.code = index;
            materials_.insert({index, data});
            mat_p = materials_.find(index);
            /* printf("0x%08x -> %d = %d, %d, %d\n", color, mat->second.index,
             * x, y, z); */
          }
          model->data[i] = mat_p->second.index;
        }
        /* printf("%d\t\t", model->data[i]); */
      }
      /* printf("\n"); */
    }
    /* printf("\n"); */
    /* printf("\n"); */
  }
  // SDF

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      for (int z = 0; z < depth; ++z) {
        int id = (z * x_size * y_size + y * x_size + x) * 3;
        if (model->data[id]) {
          continue;
        }
        int i = (z * x_size * y_size + y * x_size + x) * 3 + 1;
        int it = (z * x_size * y_size + y * x_size + x) * 3 + 2;
        int len = 255;
        int lent = 255;
        for (int y1 = 0; y1 < height; ++y1) {
          for (int x1 = 0; x1 < width; ++x1) {
            for (int z1 = 0; z1 < depth; ++z1) {
              int jd = (z1 * x_size * y_size + y1 * x_size + x1) * 3;
              if (!model->data[jd]) {
                continue;
              }
              int l = abs(y - y1) + abs(x - x1) + abs(z - z1);
              int lt = glm::max(glm::max(abs(y - y1), abs(x - x1)), abs(z - z1)) ;
              lent = glm::min(lt, lent);
              len = glm::min(l, len);
            }
          }
        }
        model->data[i] = len;
          model->data[it] = lent;
      }
    }
  }


  TexConf config = {TexConf::RGB, TexConf::UNSIGNED_BYTE, TexConf::NEAREST};
  model->texture.Setup3D(x_size, y_size, z_size, model->data, config);
  model->comp.id = model->texture.renderer_id();
  model->comp.raw_size = raw_size;
  model->comp.size = size;

  models_.insert({name, model});
  return model->comp;
}
const ModelComponent &VoxelModels::AddModel(const std::string &file_path) {
  ModelData *model = new ModelData();

  model->file_path = Engine::config().asset_folder + file_path;

  std::ifstream file(model->file_path);
  arc_assert(file.is_open(),
             ("Couldn't open model: " + model->file_path).c_str());

  file.seekg(0, std::ios::end);
  int s = file.tellg();
  logi("size {0}", s);
  file.seekg(0, std::ios::beg);

  uint8_t *buff = new uint8_t[s];
  file.read(reinterpret_cast<char *>(buff), s);
  const ogt_vox_scene *scene = ogt_vox_read_scene(buff, s);
  delete[] buff;
  file.close();

  logi("Groups: {0}", scene->num_groups);
  logi("Instances: {0}", scene->num_instances);
  logi("Layers: {0}", scene->num_layers);
  logi("Models: {0}", scene->num_models);
  glm::vec3 raw_size, size, offset;
  for (int inst = 0; inst < scene->num_instances && inst < 1; ++inst) {
    auto ogt_model = scene->models[scene->instances[inst].model_index];
    size_t x_size, y_size, z_size;
    { // get size
      logi("x: {0}", ogt_model->size_x);
      logi("y: {0}", ogt_model->size_y);
      logi("z: {0}", ogt_model->size_z);
      x_size = ogt_model->size_x;
      y_size = ogt_model->size_y;
      z_size = ogt_model->size_z;

      raw_size = {x_size, y_size, z_size};
      offset = (*((glm::mat4 *)&scene->instances[inst].transform))[3];
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
      size = {x_size, y_size, z_size};
      logi("fixed x: {0}", x_size);
      logi("fixed y: {0}", y_size);
      logi("fixed z: {0}", z_size);
    }
    size_t full_size = x_size * y_size * z_size;
    model->data = new uint8_t[full_size * 3];
    for (int i = 0; i < full_size * 3; ++i) {
      model->data[i] = 0;
    }
    for (int x = 0; x < ogt_model->size_x; ++x) {
      for (int y = 0; y < ogt_model->size_y; ++y) {
        for (int z = 0; z < ogt_model->size_z; ++z) {
          int i_in = z * ogt_model->size_x * ogt_model->size_y +
                     y * ogt_model->size_x + x;
          int i_out = (z * x_size * y_size + y * x_size + x) * 3;
          model->data[i_out] = ogt_model->voxel_data[i_in];
        }
      }
    }

    for (int y = 0; y < ogt_model->size_y; ++y) {
      for (int x = 0; x < ogt_model->size_x; ++x) {
        for (int z = 0; z < ogt_model->size_z; ++z) {
          int i = (z * x_size * y_size + y * x_size + x) * 3;
          /* int i = x * z_size * y_size + y * z_size + z; */
          uint8_t ci = model->data[i];

          if (ci) {
            auto col = scene->palette.color[ci];
            auto material = scene->materials.matl[ci];
            uint64_t index = getMatIndex({col.r, col.g, col.b}, material.alpha, material.metal, material.rough);
            auto mat = materials_.find(index);
            if (mat == materials_.end()) {
              MaterialData data;
              data.mat.color = glm::vec4{col.r, col.g, col.b, col.a} / 255.0f;
              data.index = materials_.size() + 1;
              arc_assert(data.index < 256,
                         "Too many diferent materials in scene!");
              data.code = index;
              materials_.insert({index, data});
              mat = materials_.find(index);
              /* printf("0x%08x -> %d = %d, %d, %d\n", color, mat->second.index,
               * x, y, z); */
            }
            model->data[i] = mat->second.index;
          }
          /* printf("%d\t\t", model->data[i]); */
        }
        /* printf("\n"); */
      }
      /* printf("\n"); */
      /* printf("\n"); */
    }

    for (int y = 0; y < ogt_model->size_y; ++y) {
      for (int x = 0; x < ogt_model->size_x; ++x) {
        for (int z = 0; z < ogt_model->size_z; ++z) {
          int id = (z * x_size * y_size + y * x_size + x) * 3;
          if (model->data[id]) {
            continue;
          }
          int i = (z * x_size * y_size + y * x_size + x) * 3 + 1;
          int it = (z * x_size * y_size + y * x_size + x) * 3 + 2;
          int len = 255;
          int lent = 255;
          for (int y1 = 0; y1 < ogt_model->size_y; ++y1) {
            for (int x1 = 0; x1 < ogt_model->size_x; ++x1) {
              for (int z1 = 0; z1 < ogt_model->size_z; ++z1) {
                int jd = (z1 * x_size * y_size + y1 * x_size + x1) * 3;
                if (!model->data[jd]) {
                  continue;
                }
                int l = abs(y - y1) + abs(x - x1) + abs(z - z1);
                int lt = glm::max(glm::max(abs(y - y1), abs(x - x1)), abs(z - z1)) ;
                len = glm::min(l, len);
                /* if(lt >1){ */
                /*   lt--; */
                /* } */
                lent = glm::min(lt, lent);
              }
            }
          }
          model->data[i] = len;
          model->data[it] = lent;
        }
      }
    }

    TexConf config = {TexConf::RGB, TexConf::UNSIGNED_BYTE, TexConf::NEAREST};
    model->texture.Setup3D(x_size, y_size, z_size, model->data, config);
    model->comp.id = model->texture.renderer_id();
    model->comp.raw_size = raw_size;
    model->comp.size = size;
  } 
  models_.insert({file_path, model});
  return model->comp;

}
void VoxelModels::Dispose() {
  if (models_.empty()) {
    return;
  }
  for (auto kv : models_) {
    delete[] kv.second->data;
    kv.second->texture.Dispose();
  }
}
} // namespace arc
