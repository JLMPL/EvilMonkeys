#include "ArrayBuffer.hpp"

ArrayBuffer::~ArrayBuffer()
{
    GL(glDeleteBuffers(1, &m_buffer));
}

void ArrayBuffer::init(GLenum type)
{
    m_type = type;
    
    GL(glDeleteBuffers(1, &m_buffer));
    GL(glGenBuffers(1, &m_buffer));    
}

void ArrayBuffer::setData(GLsizeiptr size, const GLvoid* data, GLenum usage)
{
    GL(glBufferData(m_type, size, data, usage));
}

void ArrayBuffer::bind() const
{
    GL(glBindBuffer(m_type, m_buffer));
}

void ArrayBuffer::unbind() const
{
    GL(glBindBuffer(m_type, 0));
}
