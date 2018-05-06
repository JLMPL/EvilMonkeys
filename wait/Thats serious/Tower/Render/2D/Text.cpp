#include "Text.hpp"
#include "Font.hpp"
#include "Core/Types.hpp"
#include "Render/Renderer2D.hpp"
#include <vector>

Text::~Text()
{
    gRenderer2D.unregisterText(this);
}

void Text::init(const Font* font)
{
    m_font = font;
}

void Text::setString(const std::string& str, f32 size)
{
    std::vector<vec2> verts;
    std::vector<vec2> coords;
    float posx = 0;

    m_width = 0;
    m_height = 0;

    m_string = str;
    m_size = size;

    f32 texel = 1.f / 512.f;

    for (u32 i = 0; i < m_string.size(); i++)
    {
        Glyph gl = m_font->getGlyph(str[i]);

        gl.xoffset *= size;
        gl.yoffset *= size;
        gl.xadvance *= size;

        vec2 vrt[] =
        {
            {0, 0},
            {gl.width * size, 0},
            {gl.width * size, gl.height * size},

            {0, 0},
            {gl.width * size, gl.height * size},
            {0, gl.height * size}
        };

        i32 minHeight = 1000;
        i32 maxHeight = -1000;

        for (u32 i = 0; i < 6; i++)
        {
            vec2 thevert = vrt[i] + vec2(posx, 0) + vec2(gl.xoffset, gl.yoffset);
            verts.push_back(thevert);

            if (thevert.x > m_width)
                m_width = thevert.x;

            if (thevert.y < minHeight)
                minHeight = thevert.y;
            if (thevert.y > maxHeight)
                maxHeight = thevert.y;
        }

        m_height = maxHeight - minHeight;

        coords.push_back(vec2(gl.x * texel             , gl.y * texel));
        coords.push_back(vec2((gl.x + gl.width) * texel, gl.y * texel));
        coords.push_back(vec2((gl.x + gl.width) * texel, (gl.y + gl.height) * texel));

        coords.push_back(vec2(gl.x * texel             , gl.y * texel));
        coords.push_back(vec2((gl.x + gl.width) * texel, (gl.y + gl.height) * texel));
        coords.push_back(vec2(gl.x * texel             , (gl.y + gl.height) * texel));

        posx += gl.xadvance * 0.75;
    }

    m_vao.init();
    m_vao.bind();

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(float) * 2 * verts.size(), &verts[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_tbo.init(GL_ARRAY_BUFFER);
    m_tbo.bind();
    m_tbo.setData(sizeof(float) * 2 * coords.size(), &coords[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_vao.unbind();
    m_vbo.unbind();
}

void Text::render() const
{
    m_vao.bind();
    GL(glDrawArrays(GL_TRIANGLES, 0, m_string.size() * 6));
    m_vao.unbind();
}

void Text::setPosition(const vec2& vec)
{
    m_pos = vec;
}

vec2 Text::getPosition() const
{
    return m_pos;
}

u32 Text::getWidth() const
{
    return m_width;
}

u32 Text::getHeight() const
{
    return m_height;
}

vec2u Text::getSize() const
{
    return {m_width, m_height};
}
