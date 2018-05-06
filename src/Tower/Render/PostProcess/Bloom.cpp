#include "Bloom.hpp"
#include "Core/Config.hpp"

void PostProcessBloom::init()
{
    m_luma.fbo.init(Framebuffer::Type::Color, Config::Display::Width, Config::Display::Height);
    m_luma.shader.loadFromFile("Shaders/Bloom/Luma.sha");

    m_add.fbo.init(Framebuffer::Type::Color, Config::Display::Width, Config::Display::Height);
    m_add.shader.loadFromFile("Shaders/Bloom/Add.sha");

    m_screenQuad.init();

    m_blur.init(0.5f);
}

void PostProcessBloom::lumaPass()
{
    m_luma.fbo.bind();
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_luma.shader.bind();
    m_luma.shader.setUniformTexture("image", 0, m_original);

    m_screenQuad.render();

    m_luma.shader.unbind();
    m_luma.fbo.unbind();

    m_luma.texture = m_luma.fbo.getTexture();
}

void PostProcessBloom::addPass()
{
    m_add.fbo.bind();
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_add.shader.bind();
    m_add.shader.setUniformTexture("image", 0, m_original);
    m_add.shader.setUniformTexture("luma", 1, m_working);

    m_screenQuad.render();

    m_add.shader.unbind();
    m_add.fbo.unbind();
}

GLuint PostProcessBloom::process(GLuint texture)
{
    m_original = texture;

    lumaPass();
    m_working = m_blur.process(m_luma.texture);
    m_working = m_blur.process(m_working);
    addPass();

    return m_add.fbo.getTexture();
}
