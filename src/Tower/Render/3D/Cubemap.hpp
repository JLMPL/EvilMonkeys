#pragma once
#include "Core/Types.hpp"
#include "../OpenGL.hpp"
#include <string>

class Cubemap
{
    public:
        Cubemap() = default;
        ~Cubemap();

        void init(u32 size, GLenum internal = GL_RGBA, GLenum basic = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);
        void initFromFiles(const std::string* files, bool filesHavePostfixes = false);

        void generateMips();

        void bind() const;
        void unbind() const;

        GLuint getId() const;

    private:
        GLuint m_cubemap = 0;
};
