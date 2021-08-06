#include "shadow_texture.h"
#include <glm/gtc/matrix_transform.hpp>
namespace arc {
void ShadowTexture::Init(uint width, uint height, uint depth,
                         const glm::vec3 &size, const glm::vec3 &pos) {

  int wt = (width ^ -width) & 255;
  arc_assert(wt == width || wt == 0,
             "Width of shadow texture has to be devisable by 2");
  int ht = (height ^ -height) & 255;
  arc_assert(ht == height || ht == 0,
             "Height of shadow texture has to be devisable by 2");
  int dt = (depth ^ -depth) & 255;
  arc_assert(dt == depth || dt == 0,
             "Depth of shadow texture has to be devisable by 2");

  width_ = width;
  height_ = height;
  depth_ = depth;
  size_ = size;
  pos_ = pos;
  data_ = new uint8_t[width_ * height_ * depth_ / 8];
  for (int i = 0; i < width_ * height_ * depth_ / 8; ++i) {
    data_[i] = 0b00000000;
  }
  texture_.Setup3D(
      width_ >> 1, height_ >> 1, depth_ >> 1, data_,
      {TexConf::R, TexConf::UNSIGNED_BYTE, TexConf::NEAREST});
}

void ShadowTexture::Destroy() {

  delete[] data_;
  texture_.Dispose();
}

void ShadowTexture::SetBits(const VoxelModel &model, const glm::mat4 &transform,
                            bool set) {
  uint8_t *mask = model.shadow_mask(0);
  auto size = model.raw_size(0);
  int width = size.x, height = size.y, depth = size.z;
  int sw=width_, sh=height_, sd = depth_;//where to start updating the texture
  int ew=0, eh=0, ed = 0;//where to start updating the texture
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      for (int z = 0; z < depth; ++z) {
        int i = z / 2 * height * width / 4 + y / 2 * width / 2 + x / 2;
        uint8_t ib = 1 << ((z & 1) * 4 + (y & 1) * 2 + (x & 1));
        uint8_t val = mask[i] & ib;
        if (val) {
          glm::vec4 in_vec = {x, y, z, 0};
          in_vec.x += -model.size(0).x / 2;
          in_vec.y += -model.size(0).y / 2;
          in_vec.z += -model.size(0).z / 2;
          glm::vec4 out_vec = transform * in_vec;
          int x2 = glm::round(out_vec.x + width_ / 2 +
                              transform[3].x *
                                  10); 
                                      
          int y2 = glm::round(out_vec.y + height_ / 2 + transform[3].y * 10);
          int z2 = glm::round(out_vec.z + depth_ / 2 + transform[3].z * 10);
          if (x2 < 0 || x2 > width_ || y2 < 0 || y2 > height_ || z2 < 0 ||
              z2 > depth_) {
            continue;
          }
          // update bounds
          sw = glm::min(sw,x2/2); 
          sh = glm::min(sh,y2/2); 
          sd = glm::min(sd,z2/2); 
          ew = glm::max(ew,x2/2); 
          eh = glm::max(eh,y2/2); 
          ed = glm::max(ed,z2/2); 

          // update array
          if (set)
            data_[(z2 / 2) * height_ * width_ / 4 + (y2 / 2) * width_ / 2 +
                  (x2 / 2)] |= 1 << ((z2 & 1) * 4 + (y2 & 1) * 2 + (x2 & 1));
          else
            data_[(z2 / 2) * height_ * width_ / 4 + (y2 / 2) * width_ / 2 +
                  (x2 / 2)] = 0;
        }
      }
    }
  }
  if(sh == height_) return;

  texture_.set_data(data_, 0,0,0,width_/2,height_/2,depth_/2);
}
} // namespace arc
