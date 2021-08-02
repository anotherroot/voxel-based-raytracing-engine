//
// Created by sam on 12/25/20.
//

#ifndef ARC_ENGINE_OPENGL_SHADER_H
#define ARC_ENGINE_OPENGL_SHADER_H

#include "core.h"
#include <glm.hpp>
namespace glm{

/* struct vec2; */
/* struct vec3; */
/* struct vec4; */
/* struct mat3; */
/* struct mat4; */

}
namespace arc {
class Shader {
public:
  void Setup(const std::string &name, const std::string &vertex_source,
         const std::string &fragment_source);
  void Setup(std::string file_path);
  void Dispose();
  ~Shader(){Dispose();}
  operator bool() const { return id_; }

  void Bind() const;
  void Unbind() const;

  void SetFragOutput(unsigned int color, const std::string &name);

  void SetInt(const std::string &name, int value);
  void SetIntArray(const std::string &name, int *values, unsigned int count);
  void SetFloat(const std::string &name, float value);
  void SetFloat2(const std::string &name, const glm::vec2 &value);
  void SetFloat3(const std::string &name, const glm::vec3 &value);
  void SetFloat4(const std::string &name, const glm::vec4 &value);
  void SetMat4(const std::string &name, const glm::mat4 &value);

  const std::string &name() const { return name_; }
  void UploadUniformInt(const std::string &name, int value) const;
  void UploadUniformIntArray(const std::string &name, int *values,
                             unsigned int count) const;
  void UploadUniformFloat(const std::string &name, float value) const;
  void UploadUniformFloat2(const std::string &name,
                           const glm::vec2 &values) const;
  void UploadUniformFloat3(const std::string &name,
                           const glm::vec3 &values) const;
  void UploadUniformFloat4(const std::string &name,
                           const glm::vec4 &values) const;

  void UploadUniformMat3(const std::string &name,
                         const glm::mat3 &matrix) const;
  void UploadUniformMat4(const std::string &name,
                         const glm::mat4 &matrix) const;

private:
  void Compile(const std::unordered_map<uint, std::string> &shader_sources);
  std::string ReadFile(const std::string &file_path);
  std::unordered_map<uint, std::string> PreProcess(const std::string &source);

private:
  unsigned int id_{0};
  std::string name_;
};
} // namespace arc

#endif // ARC_ENGINE_OPENGL_SHADER_H
