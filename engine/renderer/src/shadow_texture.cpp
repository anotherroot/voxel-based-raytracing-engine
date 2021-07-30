#include "shadow_texture.h"
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
  for (int i = 0; i < width_ >> 1; ++i) {
    for (int j = 0; j < height_ >> 1; ++j) {
      for (int k = 0; k < depth_ >> 1; ++k) {
        data_[i * height_ * depth_ / 4 + j * depth_ / 2 + k] = 0b00000000;
        if (i == width_ >> 2 && j == width_ >> 2)
          data_[i * height_ * depth_ / 4 + j * depth_ / 2 + k] = 255;
        if (k == width_ >> 2 && j == width_ >> 2)
          data_[i * height_ * depth_ / 4 + j * depth_ / 2 + k] = 255;
      }
    }
  }
  texture_.Setup(
      width_ >> 1, height_ >> 1, depth_ >> 1, data_,
      {TextureConfig::R, TextureConfig::UNSIGNED_BYTE, TextureConfig::NEAREST});
}

void ShadowTexture::Destroy() {
  delete[] data_;
  texture_.Dispose();
}

void ShadowTexture::SetBits(const VoxelModel &model,
                            const glm::vec3 &position) {
  uint8_t *data = model.data(0);
  glm::vec3 size = model.size(0);
  int width = size.x, height = size.z, depth = size.y;
  glm::vec3 start = glm::vec3(width_ >> 1, height_ >> 1, depth_ >> 1) +
                    position / 0.1f; // TODO: 0.1f is the size of the voxel..

  for (int i = 0; i < width; ++i) {
    int si = i + start.x;
    if (si >= width_ || si < 0)
      continue;
    for (int j = 0; j < height; ++j) {
      int sj = j + start.y;
      if (sj >= height_ || sj < 0)
        continue;
      for (int k = 0; k < depth; ++k) {
        int sk = k + start.z;
        if (sk >= depth_ || sk < 0)
          continue;
        uint8_t d = data[i * height * depth + j * depth + k];
        if (d) {

          /* data_[(si >> 1) * height * (depth >> 2) + (sj >> 1) * (depth >> 1) + */
                /* (sk >> 1)] |= 1 << ((si & 1) * 4 + (sj & 1) * 2 + sk & 1); */
          /* logi("{0}, {1}, {2} => {3} {4}, {5}, => {6}", i, j, k, si, sj, sk, */
          /*      data_[(si >> 1) * height * (depth >> 2) + */
                     /* (sj >> 1) * (depth >> 1) + (sk >> 1)]); */
        }
      }
    }
  }
  data_[((width_ >> 2) + 1) * height_ * depth_ / 4 + (height_ >> 2) * depth_ / 2 +
        (depth_ >> 2) + 1] = 128;
  int ex = glm::min((int)start.x + width, (int)width_);
  int ey = glm::min((int)start.y + height, (int)height_);
  int ez = glm::min((int)start.z + depth, (int)depth_);
  texture_.set_data(data_, 0);
}
} // namespace arc
