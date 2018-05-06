#include "Renderer2D.hpp"
#include "2D/Text.hpp"
#include "2D/Rectangle.hpp"
#include "Core/Config.hpp"

Renderer2D gRenderer2D;

void Renderer2D::init()
{
    m_shaders.rectangle.loadFromFile("Shaders/Interface/Rectangle.sha");
    m_shaders.text.loadFromFile("Shaders/Interface/Text.sha");

    m_proj = math::ortho(0.f, float(Config::Display::Width), float(Config::Display::Height), 0.f);

    m_textture.loadFromFile("Fonts/dis.png");
}

void Renderer2D::refresh()
{
    m_proj = math::ortho(0.f, float(Config::Display::Width), float(Config::Display::Height), 0.f);
}

void Renderer2D::registerRectangle(const Rectangle* rect)
{
    m_rectangles.add(rect);
}

void Renderer2D::unregisterRectangle(const Rectangle* rect)
{
    m_rectangles.remove(rect);
}

void Renderer2D::registerText(const Text* text)
{
    m_texts.add(text);
}

void Renderer2D::unregisterText(const Text* text)
{
    m_texts.remove(text);
}

void Renderer2D::update(seconds deltaTime)
{
}

void Renderer2D::render()
{
    GL(glDisable(GL_DEPTH_TEST));
    GL(glDisable(GL_CULL_FACE));
    GL(glEnable(GL_BLEND));
    GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    m_shaders.rectangle.bind();
    m_shaders.rectangle.setUniformMatrix("uProj", m_proj);

    for (u32 i = 0; i < m_rectangles.size(); i++)
    {
        m_shaders.rectangle.setUniform2f("uPos", m_rectangles[i]->getPosition());
        m_shaders.rectangle.setUniform4f("color", m_rectangles[i]->getColor());

        m_rectangles[i]->render();
    }

    m_shaders.rectangle.unbind();

    m_shaders.text.bind();
    m_shaders.text.setUniformMatrix("uProj", m_proj);
    m_shaders.text.setUniformTexture("uImage", 0, m_textture);

    for (u32 i = 0; i < m_texts.size(); i++)
    {
        m_shaders.text.setUniform2f("uPos", m_texts[i]->getPosition());
        m_texts[i]->render();
    }

    m_shaders.text.unbind();

    GL(glDisable(GL_BLEND));
    GL(glEnable(GL_CULL_FACE));
    GL(glEnable(GL_DEPTH_TEST));
}

void Renderer2D::clearAllData()
{
    m_texts.clear();
    m_rectangles.clear();
}
