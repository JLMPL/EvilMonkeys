#pragma once
#include "Core/Types.hpp"
#include "Render/OpenGL.hpp"

class Framebuffer
{
    public:
        enum class Type
        {
            Color,
            Depth
        };

        ~Framebuffer();

        void init(Type type, u32 width, u32 height, bool forShadow = false, GLint internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum gltype = GL_UNSIGNED_BYTE);
        void bind() const;
        void unbind() const;

        void bindRead() const;
        void bindWrite() const;

        u32 getWidth() const;
        u32 getHeight() const;

        GLuint getTexture() const;
        GLuint getDepth() const;

    private:
        void createColor();
        void createDepth(bool forShadow);

    private:
        GLuint    m_fbo = 0;
        GLuint    m_texture = 0;
        GLuint    m_depth = 0;
        GLuint    m_buffer = 0;

        GLint     m_internalFormat = 0;
        GLenum    m_format = 0;
        GLenum    m_type = 0;

        u32    m_width = 0;
        u32    m_height = 0;
};
