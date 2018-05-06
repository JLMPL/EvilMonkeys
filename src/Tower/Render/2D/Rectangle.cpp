#include "Rectangle.hpp"

void Rectangle::init(const vec2& size)
{
    setSize(size);
}

void Rectangle::setSize(const vec2& size)
{
    const float verts[] =
    {
        0,0,
        0,size.y,
        size.x, size.y,

        0,0,
        size.x, size.y,
        size.x, 0
    };

    const float coords[] =
    {
        0,1,
        0,0,
        1,0,

        0,1,
        1,0,
        1,1
    };

    m_vao.init();
    m_vao.bind();

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(float) * 2 * 6, verts, GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_tbo.init(GL_ARRAY_BUFFER);
    m_tbo.bind();
    m_tbo.setData(sizeof(float) * 2 * 6, coords, GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_vao.unbind();
    m_vbo.unbind();
}

void Rectangle::render() const
{
    m_vao.bind();
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    m_vao.unbind();
}

void Rectangle::setColor(const vec4& color)
{
    m_color = color;
}

vec4 Rectangle::getColor() const
{
    return m_color;
}

void Rectangle::setPosition(const vec2& pos)
{
    m_pos = pos;
}

vec2 Rectangle::getPosition() const
{
    return m_pos;
}
