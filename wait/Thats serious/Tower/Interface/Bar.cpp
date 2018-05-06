#include "Bar.hpp"
#include "Render/Renderer2D.hpp"

void Bar::init(const vec2& pos, const vec2& size, const vec3& color, u32 maxValue, bool leftie)
{
    m_leftie = leftie;

    m_width = size.x;
    m_height = size.y;

    m_maxValue = maxValue;

    m_rect.init(size);
    m_rect.setColor(vec4(vec3(color), 1.f));
    m_rect.setPosition(pos);

    m_back.init(size);
    m_back.setColor(vec4(vec3(0.f), 0.5f));
    m_back.setPosition(pos);

    gRenderer2D.registerRectangle(&m_back);
    gRenderer2D.registerRectangle(&m_rect);

    update();
}

void Bar::update()
{
    float step = float(m_width) / float(m_maxValue);
    float w = float(m_value) * step;
    m_rect.setSize({w, m_height});

    if (m_leftie)
    {
        m_rect.setPosition({m_back.getPosition().x + (m_width - w), m_back.getPosition().y});
    }

    // if (m_value == 0)
    //     m_rect.setSize({1, m_height});
}

void Bar::show()
{
    if (!m_isShown)
    {
        // g_renderer.addOverlayData(&m_date);
        // g_renderer.addOverlayData(&m_data);

        m_isShown = true;
    }
}

void Bar::hide()
{
    if (m_isShown)
    {
        // g_renderer.removeOverlayData(&m_date);
        // g_renderer.removeOverlayData(&m_data);

        m_isShown = false;
    }
}

void Bar::setMaxValue(u32 value)
{
    m_maxValue = math::max(value, 1u);

    update();
}

u32 Bar::getMaxValue()
{
    return m_maxValue;
}

void Bar::setValue(i32 value)
{
    if (value < 0)
        m_value = 0;
    else
        m_value = value;

    update();
}

u32 Bar::getValue()
{
    return m_value;
}
