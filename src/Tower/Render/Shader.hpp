#pragma once
#include "Core/Types.hpp"
#include "Core/Math3d.hpp"
#include "Render/OpenGL.hpp"
#include <string>
#include <memory>

class Texture;
class Cubemap;

class Shader
{
    public:
        using Ptr = std::shared_ptr<Shader>;

        ~Shader();

        void loadFromFile(const char* path);

        GLint getUniformLocation(const char* unif) const;

        void setUniformTexture(const std::string& name, u32 index, const Texture& tex) const;
        void setUniformTexture(const std::string& name, u32 index, const Cubemap& tex) const;
        void setUniformTexture(const std::string& name, u32 index, GLuint tex) const;
        void setUniformCubemap(const std::string& name, u32 index, GLuint tex) const;

        void setUniform1i(const char* name, i32 val) const;
        void setUniform1f(const char* name, float val) const;
        void setUniform2f(const char* name, const vec2& vec) const;
        void setUniform3f(const char* name, float x, float y, float z) const;
        void setUniform3f(const char* name, const vec3& vec) const;
        void setUniform4f(const char* name, const vec4& vec) const;
        void setUniformMatrix(const char* name, const mat4& mat) const;

        void bind() const;
        void unbind() const;

    private:
        auto loadSource(const char* path);
        GLuint createShader(const std::string& source, GLuint mode);

    private:
        GLuint    m_program = 0;
        GLuint    m_vertex = 0;
        GLuint    m_fragment = 0;
};
