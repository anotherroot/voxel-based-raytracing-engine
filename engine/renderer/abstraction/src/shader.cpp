//
// Created by sam on 12/25/20.
//

#include "shader.h"
#include <fstream>
#include <glad/glad.h>
#include <gtc/type_ptr.hpp>
namespace arc {
static GLenum ShaderTypeFromString(const std::string &type) {
  if (type == "vertex")
    return GL_VERTEX_SHADER;
  if (type == "fragment" || type == "pixel")
    return GL_FRAGMENT_SHADER;
  arc_core_assert(false, "Unknown shader type!");
}

void Shader::Setup(const std::string &name, const std::string &vertex_source,
                   const std::string &fragment_source) {
  arc_assert(!(*this), "Shader Setup failed! Shader already exists");

  name_ = name;
  std::unordered_map<GLenum, std::string> sources;
  sources[GL_VERTEX_SHADER] = vertex_source;
  sources[GL_FRAGMENT_SHADER] = fragment_source;
  Compile(sources);
}

void Shader::Setup(std::string file_path) {
  arc_assert(!(*this), "Shader Setup failed! Shader already exists");
  std::string source = ReadFile(file_path + "/opengl.glsl");
  auto shader_sources = PreProcess(source);
  Compile(shader_sources);

  // extract name from file path
  auto last_slash = file_path.find_last_of("/\\");
  last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
  auto count = file_path.size() - last_slash;
  name_ = file_path.substr(last_slash, count);
}
void Shader::Dispose() {
  if (*this) {
    glDeleteProgram(id_);
    id_ = 0;
  }
}

std::string Shader::ReadFile(const std::string &file_path) {
  std::string result;
  std::ifstream in(file_path, std::ios::in | std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    result.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&result[0], result.size());

    in.close();
  } else {
    arc_core_assert(false, "Could not open file: {}", file_path);
  }

  return result;
}

std::unordered_map<uint, std::string>
Shader::PreProcess(const std::string &source) {
  std::unordered_map<uint, std::string> shader_sources;

  const char *type_token = "#type";
  size_t type_token_length = strlen(type_token);
  size_t pos = source.find(type_token, 0);
  while (pos != std::string::npos) {
    size_t eol = source.find_first_of("\r\n", pos);
    arc_core_assert(eol != std::string::npos, "Syntax error in shader file!");
    size_t begin = pos + type_token_length + 1;
    std::string type = source.substr(begin, eol - begin);
    arc_core_assert(type == "vertex" || type == "fragment" || type == "pixel",
                    "Invalid shader type specified!");

    size_t next_line_pos = source.find_first_of("\r\n", eol);
    pos = source.find(type_token, next_line_pos);
    shader_sources[ShaderTypeFromString(type)] =
        source.substr(next_line_pos, pos - (next_line_pos == std::string::npos
                                                ? source.size() - 1
                                                : next_line_pos));
  }
  return shader_sources;
}

void Shader::Compile(
    const std::unordered_map<uint, std::string> &shader_sources) {
  GLuint program = glCreateProgram();
  arc_core_assert(shader_sources.size() <= 2,
                  "Only support two shaders for now!");
  std::array<uint, 2> gl_shader_ids;
  int gl_shader_id_index = 0;
  for (auto &kv : shader_sources) {
    GLenum type = kv.first;

    GLuint shader = glCreateShader(type);

    const GLchar *source = kv.second.c_str();
    glShaderSource(shader, 1, &source, 0);

    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

      // We don't need the shader anymore.
      glDeleteShader(shader);

      // Use the infoLog as you see fit.
      logce("{0}", infoLog.data());
      arc_core_assert(false, "Shader compilation failure!");
      break;
    }
    glAttachShader(program, shader);
    gl_shader_ids[gl_shader_id_index++] = shader;
  }

  id_ = program;

  // Link our program
  glLinkProgram(id_);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(id_, GL_LINK_STATUS, (int *)&isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(id_, maxLength, &maxLength, &infoLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(id_);
    // Don't leak shaders either.
    for (auto shader : gl_shader_ids) {
      glDeleteShader(shader);
    }

    logce("{0}", infoLog.data());
    arc_core_assert(false, "Linking of shaders failed!");
  }

  // Always detach shaders after a successful link.
  for (auto shader : gl_shader_ids) {
    glDetachShader(id_, shader);
  }
}

void Shader::Bind() const { glUseProgram(id_); }

void Shader::Unbind() const { glUseProgram(0); }

void Shader::SetFragOutput(unsigned int color, const std::string &name) {
  glBindFragDataLocation(id_, color, name.c_str());
}

void Shader::SetInt(const std::string &name, int value) {
  UploadUniformInt(name, value);
}

void Shader::SetIntArray(const std::string &name, int *values,
                         unsigned int count) {
  UploadUniformIntArray(name, values, count);
}

void Shader::SetFloat(const std::string &name, float value) {
  UploadUniformFloat(name, value);
}
void Shader::SetFloat2(const std::string &name, const glm::vec2 &value) {
  UploadUniformFloat2(name, value);
}
void Shader::SetFloat3(const std::string &name, const glm::vec3 &value) {
  UploadUniformFloat3(name, value);
}

void Shader::SetFloat4(const std::string &name, const glm::vec4 &value) {
  UploadUniformFloat4(name, value);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &value) {
  UploadUniformMat4(name, value);
}

void Shader::UploadUniformInt(const std::string &name, int value) const {
  GLint location = glGetUniformLocation(id_, name.c_str());
  glUniform1i(location, value);
}

void Shader::UploadUniformIntArray(const std::string &name, int *values,
                                   unsigned int count) const {
  GLint location = glGetUniformLocation(id_, name.c_str());
  glUniform1iv(location, count, values);
}

void Shader::UploadUniformFloat(const std::string &name, float value) const {
  GLint location = glGetUniformLocation(id_, name.c_str());
  glUniform1f(location, value);
}

void Shader::UploadUniformFloat2(const std::string &name,
                                 const glm::vec2 &values) const {
  GLint location = glGetUniformLocation(id_, name.c_str());
  glUniform2f(location, values.x, values.y);
}

void Shader::UploadUniformFloat3(const std::string &name,
                                 const glm::vec3 &values) const {
  GLint location = glGetUniformLocation(id_, name.c_str());
  glUniform3f(location, values.x, values.y, values.z);
}

void Shader::UploadUniformFloat4(const std::string &name,
                                 const glm::vec4 &values) const {
  GLint location = glGetUniformLocation(id_, name.c_str());
  glUniform4f(location, values.x, values.y, values.z, values.w);
}

void Shader::UploadUniformMat3(const std::string &name,
                               const glm::mat3 &matrix) const {
  GLint location = glGetUniformLocation(id_, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UploadUniformMat4(const std::string &name,
                               const glm::mat4 &matrix) const {
  GLint location = glGetUniformLocation(id_, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
} // namespace arc
