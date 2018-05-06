#pragma once
#include "OpenGL.hpp"

class ArrayBuffer
{
    public:
        ArrayBuffer() = default;
        ~ArrayBuffer();

        void init(GLenum type);

        void setData(GLsizeiptr size, const GLvoid* data, GLenum usage);

        void bind() const;
        void unbind() const;

    private:
        GLenum m_type;
        GLuint m_buffer = 0;
};
