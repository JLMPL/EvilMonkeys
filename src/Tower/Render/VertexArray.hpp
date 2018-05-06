#pragma once
#include "OpenGL.hpp"

class VertexArray
{
    public:
        VertexArray() = default;
        ~VertexArray();

        void init();

        void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

        void bind() const;
        void unbind() const;

    private:
        GLuint m_vao = 0;
};
