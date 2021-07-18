//
// Created by sam on 12/25/20.
//

#ifndef ARC_ENGINE_OPENGL3_SHADER_H
#define ARC_ENGINE_OPENGL3_SHADER_H

#include "../../core/include/core.h"
#include "../../renderer/include/shader.h"

namespace arc{
    class OpenGL3Shader: public ShaderClass{
    public:
        OpenGL3Shader(const std::string& name, const std::string& vertex_source, const std::string& fragment_source);
        OpenGL3Shader(std::string file_path);

        virtual ~OpenGL3Shader();

        void Bind() const override ;
        void Unbind() const override ;

        void SetFragOutput(unsigned int color, const std::string& name);

        void SetInt(const std::string &name,int value) override;
        void SetIntArray(const std::string &name,int* values, unsigned int count) override;
        void SetFloat(const std::string &name,float value) override;
        void SetFloat2(const std::string &name,const glm::vec2& value) override;
        void SetFloat3(const std::string &name,const glm::vec3& value) override;
        void SetFloat4(const std::string &name,const glm::vec4& value) override;
        void SetMat4(const std::string &name,const glm::mat4& value) override;



        const std::string& name() const override {return name_;}
        void UploadUniformInt(const std::string& name, int value);
        void UploadUniformIntArray(const std::string& name, int* values, unsigned int count);
        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    private:
        void Compile(const std::unordered_map<uint , std::string>& shader_sources);
        std::string ReadFile(const std::string& file_path);
        std::unordered_map<uint, std::string> PreProcess(const std::string& source);

    private:
        unsigned  int renderer_id_;
        std::string name_;

    };
}



#endif //ARC_ENGINE_OPENGL3_SHADER_H
