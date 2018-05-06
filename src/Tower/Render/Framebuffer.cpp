#include "Framebuffer.hpp"
#include "Core/Log.hpp"
#include "Core/Config.hpp"
#include <stdlib.h>

Framebuffer::~Framebuffer()
{
    GL(glDeleteFramebuffers(1, &m_fbo));
    GL(glDeleteTextures(1, &m_texture));
    GL(glDeleteRenderbuffers(1, &m_buffer));
}

void Framebuffer::createColor()
{
    GL(glGenTextures(1, &m_texture));
    GL(glBindTexture(GL_TEXTURE_2D, m_texture));

    GL(glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, 0));

    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0));
    GL(glBindTexture(GL_TEXTURE_2D, 0));

    GL(glDrawBuffer(GL_COLOR_ATTACHMENT0));

    GL(glGenRenderbuffers(1, &m_buffer));
    GL(glBindRenderbuffer(GL_RENDERBUFFER, m_buffer));
    GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height));
    GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_buffer));
    GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void Framebuffer::createDepth(bool forShadow)
{
    GL(glGenTextures(1, &m_depth));
    GL(glBindTexture(GL_TEXTURE_2D, m_depth));
    GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    if (forShadow)
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));

    GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0));
    GL(glBindTexture(GL_TEXTURE_2D, 0));

    // GL(glDrawBuffer(GL_NONE));
    // GL(glReadBuffer(GL_NONE));
}

void Framebuffer::init(Type type, u32 width, u32 height,
    bool forShadow, GLint internalFormat, GLenum format, GLenum gltype
    )
{
    {
        GL(glDeleteFramebuffers(1, &m_fbo));
        GL(glDeleteTextures(1, &m_texture));
        GL(glDeleteRenderbuffers(1, &m_buffer));
    }

    GL(glGenFramebuffers(1, &m_fbo));
    GL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

    m_internalFormat = internalFormat;
    m_format = format;
    m_type = gltype;

    m_width = width;
    m_height = height;

    if (type == Type::Color)
        createColor();
    else
        createDepth(forShadow);

    GL(int erre = glCheckFramebufferStatus(GL_FRAMEBUFFER));

    if(erre != GL_FRAMEBUFFER_COMPLETE)
    {
        Log::log(Log::Type::Error, "Framebuffer is not OK, status = %d\n", erre);
        exit(EXIT_FAILURE);
    }

    GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void Framebuffer::bind() const
{
    GL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    GL(glViewport(0, 0, m_width, m_height));
}

void Framebuffer::unbind() const
{
    GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL(glViewport(0, 0, Config::Display::Width, Config::Display::Height));
}

void Framebuffer::bindRead() const
{
    GL(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo));
}

void Framebuffer::bindWrite() const
{
    GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo));
}

u32 Framebuffer::getWidth() const
{
    return m_width;
}

u32 Framebuffer::getHeight() const
{
    return m_height;
}

GLuint Framebuffer::getTexture() const
{
    return m_texture;
}

GLuint Framebuffer::getDepth() const
{
    return m_depth;
}
