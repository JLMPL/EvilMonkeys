#include "DepthOfField.hpp"
#include "Core/Config.hpp"

void PostProcessDof::init()
{
    m_combine.fbo.init(Framebuffer::Type::Color, Config::Display::Width, Config::Display::Height);
    m_combine.shader.loadFromFile("Shaders/DepthOfField/DepthOfField.sha");

    m_bokeh.fbo.init(Framebuffer::Type::Color, Config::Display::Width * 0.6f, Config::Display::Height * 0.6f);
    m_bokeh.shader.loadFromFile("Shaders/DepthOfField/Bokeh.sha");

    m_screenQuad.init();
}

GLuint PostProcessDof::process(GLuint tex, GLuint depth)
{
    m_original = tex;
    m_depth = depth;

    blurFar();
    combine();

    return m_combine.texture;
}

void PostProcessDof::blurFar()
{
    m_bokeh.fbo.bind();
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_bokeh.shader.bind();
    m_bokeh.shader.setUniformTexture("image", 0, m_original);
    m_bokeh.shader.setUniform1f("width", m_bokeh.fbo.getWidth());
    m_bokeh.shader.setUniform1f("height", m_bokeh.fbo.getHeight());

    m_screenQuad.render();

    m_bokeh.shader.unbind();
    m_bokeh.fbo.unbind();

    m_bokeh.texture = m_bokeh.fbo.getTexture();
}

void PostProcessDof::combine()
{
    m_combine.fbo.bind();
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_combine.shader.bind();
    m_combine.shader.setUniformTexture("image", 0, m_original);
    m_combine.shader.setUniformTexture("blurred", 1, m_bokeh.texture);
    m_combine.shader.setUniformTexture("depth", 2, m_depth);

    m_screenQuad.render();

    m_combine.shader.unbind();
    m_combine.fbo.unbind();

    m_combine.texture = m_combine.fbo.getTexture();
}
