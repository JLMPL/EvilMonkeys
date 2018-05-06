#include "Blur.hpp"
#include "Core/Config.hpp"

constexpr float bigKernel[] =
{
    0.004481,
    0.008089,
    0.013722,
    0.021874,
    0.032768,
    0.046128,
    0.061021,
    0.075856,
    0.088613,
    0.097274,
    0.100346,
    0.097274,
    0.088613,
    0.075856,
    0.061021,
    0.046128,
    0.032768,
    0.021874,
    0.013722,
    0.008089,
    0.004481
};

constexpr float smallKernel[] =
{
    0.153388,
    0.221461,
    0.250301,
    0.221461,
    0.153388
};

void PostProcessBlur::init(float factor, KernelSize kernelSize)
{
    m_kernelSize = kernelSize;

    m_horizontal.fbo.init(Framebuffer::Type::Color, Config::Display::Width * factor, Config::Display::Height * factor);
    m_vertical.fbo.init(Framebuffer::Type::Color, Config::Display::Width * factor, Config::Display::Height * factor);

    if (m_kernelSize == KernelSize::Big)
    {
        m_horizontal.shader.loadFromFile("Shaders/Interface/HorizontalBlur.sha");
        m_vertical.shader.loadFromFile("Shaders/Interface/VerticalBlur.sha");
    }
    else
    {
        m_horizontal.shader.loadFromFile("Shaders/Interface/HorizontalBlur2.sha");
        m_vertical.shader.loadFromFile("Shaders/Interface/VerticalBlur2.sha");
    }

    m_screenQuad.init();
}

void PostProcessBlur::horizontalPass()
{
    m_horizontal.fbo.bind();
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_horizontal.shader.bind();
    m_horizontal.shader.setUniformTexture("image", 0, m_original);
    m_horizontal.shader.setUniform1i("textureWidth", m_horizontal.fbo.getWidth());

    if (m_kernelSize == KernelSize::Big)
    {
        for (int i = 0; i < 21; i++)
        {
            std::string index = "kernel[" + std::to_string(i) + "]";
            m_horizontal.shader.setUniform1f(index.c_str(), bigKernel[i]);
        }
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            std::string index = "kernel[" + std::to_string(i) + "]";
            m_horizontal.shader.setUniform1f(index.c_str(), smallKernel[i]);
        }
    }

    m_screenQuad.render();

    m_horizontal.shader.unbind();
    m_horizontal.fbo.unbind();

    m_horizontal.texture = m_horizontal.fbo.getTexture();
}

void PostProcessBlur::verticalPass()
{
    m_vertical.fbo.bind();
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_vertical.shader.bind();
    m_vertical.shader.setUniformTexture("image", 0, m_horizontal.texture);
    m_vertical.shader.setUniform1i("textureHeight", m_vertical.fbo.getHeight());

    if (m_kernelSize == KernelSize::Big)
    {
        for (int i = 0; i < 21; i++)
        {
            std::string index = "kernel[" + std::to_string(i) + "]";
            m_vertical.shader.setUniform1f(index.c_str(), bigKernel[i]);
        }
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            std::string index = "kernel[" + std::to_string(i) + "]";
            m_vertical.shader.setUniform1f(index.c_str(), smallKernel[i]);
        }
    }

    m_screenQuad.render();

    m_vertical.shader.unbind();
    m_vertical.fbo.unbind();

    m_vertical.texture = m_vertical.fbo.getTexture();
}

GLuint PostProcessBlur::process(GLuint texture)
{
    m_original = texture;

    horizontalPass();
    verticalPass();

    return m_vertical.fbo.getTexture();
}
