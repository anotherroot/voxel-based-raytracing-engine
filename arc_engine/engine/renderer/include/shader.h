//
// Created by sam on 12/25/20.
//

#ifndef ARC_ENGINE_SHADER_H
#define ARC_ENGINE_SHADER_H

#include <string>
#include <unordered_map>
#include <glm.hpp>
#include "core.h"

namespace arc {
    class ShaderClass {
    public:

        virtual ~ShaderClass() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void SetFragOutput(unsigned int color, const std::string &name) = 0;

        virtual void SetInt(const std::string &name, int value) = 0;

        virtual void SetIntArray(const std::string &name, int *values, unsigned int count) = 0;

        virtual void SetFloat(const std::string &name, float value) = 0;

        virtual void SetFloat2(const std::string &name, const glm::vec2 &value) = 0;

        virtual void SetFloat3(const std::string &name, const glm::vec3 &value) = 0;

        virtual void SetFloat4(const std::string &name, const glm::vec4 &value) = 0;

        virtual void SetMat4(const std::string &name, const glm::mat4 &value) = 0;


        virtual const std::string &name() const = 0;


    };

    class Shader : public Caller {
    private:
        typedef void(VoidMemberFn)(void *);

        typedef void(VoidMemberUintStrFn)(void *, uint color, const std::string name);

        typedef void(VoidMemberStrIntFn)(void *, const std::string &, int);

        typedef void(VoidMemberStrIntUintFn)(void *, const std::string &, int *, uint);

        typedef void(VoidMemberStrFloatFn)(void *, const std::string &, float);

        typedef void(VoidMemberStrVec2Fn)(void *, const std::string &, const glm::vec2 &);

        typedef void(VoidMemberStrVec3Fn)(void *, const std::string &, const glm::vec3 &);

        typedef void(VoidMemberStrVec4Fn)(void *, const std::string &, const glm::vec4 &);

        typedef void(VoidMemberStrMat4Fn)(void *, const std::string &, const glm::mat4 &);

        typedef const std::string &(StrMemberFn)(void *);

        ShaderClass *shader_{nullptr};
        VoidMemberFn *bind_fn_;
        VoidMemberFn *unbind_fn_;
        VoidMemberUintStrFn *set_frag_output_fn_;
        VoidMemberStrIntFn *set_int_fn_;
        VoidMemberStrIntUintFn *set_int_array_fn_;
        VoidMemberStrFloatFn *set_float_fn_;
        VoidMemberStrVec2Fn *set_float2_fn_;
        VoidMemberStrVec3Fn *set_float3_fn_;
        VoidMemberStrVec4Fn *set_float4_fn_;
        VoidMemberStrMat4Fn *set_mat4_fn_;
        StrMemberFn *name_fn_;

        void BindFn() {
            bind_fn_ = (VoidMemberFn *) GetVTable(shader_)[2];
            unbind_fn_ = (VoidMemberFn *) GetVTable(shader_)[3];
            set_frag_output_fn_ = (VoidMemberUintStrFn *) GetVTable(shader_)[4];
            set_int_fn_ = (VoidMemberStrIntFn *) GetVTable(shader_)[5];
            set_int_array_fn_ = (VoidMemberStrIntUintFn *) GetVTable(shader_)[6];
            set_float_fn_ = (VoidMemberStrFloatFn *) GetVTable(shader_)[7];
            set_float2_fn_ = (VoidMemberStrVec2Fn *) GetVTable(shader_)[8];
            set_float3_fn_ = (VoidMemberStrVec3Fn *) GetVTable(shader_)[9];
            set_float4_fn_ = (VoidMemberStrVec4Fn *) GetVTable(shader_)[10];
            set_mat4_fn_ = (VoidMemberStrMat4Fn *) GetVTable(shader_)[11];
        }

    public:
        void Create(const std::string &name, const std::string &vertex_source, const std::string &fragment_source);

        void Create(const std::string &file_path);

        ~Shader();

        void Dispose() {
            if (shader_ != nullptr) {
                delete shader_;
                shader_ = nullptr;
            }
        }


        inline void Bind() const {
            bind_fn_(shader_);
        }

        inline void Unbind() const {
            unbind_fn_(shader_);
        }

        inline void SetFragOutput(unsigned int color, const std::string &name) {
            set_frag_output_fn_(shader_, color, name);
        }

        inline void SetInt(const std::string &name, int value) {
            set_int_fn_(shader_, name, value);
        }

        inline void SetIntArray(const std::string &name, int *values, unsigned int count) {
            set_int_array_fn_(shader_, name, values, count);
        }

        inline void SetFloat(const std::string &name, float value) {
            set_float_fn_(shader_, name, value);
        }

        inline void SetFloat2(const std::string &name, const glm::vec2 &value) {
            set_float2_fn_(shader_, name, value);
        }

        inline void SetFloat3(const std::string &name, const glm::vec3 &value) {
            set_float3_fn_(shader_, name, value);
        }


        inline void SetFloat4(const std::string &name, const glm::vec4 &value) {
            set_float4_fn_(shader_, name, value);
        }

        inline void SetMat4(const std::string &name, const glm::mat4 &value) {
            set_mat4_fn_(shader_, name, value);
        }


        inline const std::string &name() const {
            return name_fn_(shader_);
        }

    };

}


#endif //ARC_ENGINE_SHADER_H
