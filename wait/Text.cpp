#include "Text.hpp"
#include "Core/Types.hpp"

Text::~Text()
{
    GL(glDeleteTextures(1, &m_texture));

    GL(glDeleteVertexArrays(1, &m_vao));
    GL(glDeleteBuffers(1, &m_vbo));
    GL(glDeleteBuffers(1, &m_cbo));
}

void Text::init(const Font* font)
{
    m_font = font;
}

void Text::renderTexture()
{
    SDL_Surface* surf = TTF_RenderText_Blended(m_font->get(), m_text.c_str(), {255,255,255});
    m_width = surf->w;
    m_height = surf->h;

    GL(glDeleteTextures(1, &m_texture));

    GL(glGenTextures(1, &m_texture));
    GL(glBindTexture(GL_TEXTURE_2D, m_texture));

    GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels));

    SDL_FreeSurface(surf);

    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));

    GL(glGenerateMipmap(GL_TEXTURE_2D));
    GL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Text::setupBuffers()
{
    float wi = float(m_width);
    float hi = float(m_height);

    float verts[] = {
        0,  0,
        wi, 0,
        0,  hi,

        wi, hi,
        wi, 0,
        0,  hi
    };

    float coords[] = {
        0, 0,
        1, 0,
        0, 1,

        1, 1,
        1, 0,
        0, 1
    };

    GL(glDeleteVertexArrays(1, &m_vao));
    GL(glDeleteBuffers(1, &m_vbo));
    GL(glDeleteBuffers(1, &m_cbo));

    GL(glGenVertexArrays(1, &m_vao));
    GL(glBindVertexArray(m_vao));

    GL(glGenBuffers(1, &m_vbo));
    GL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, &verts[0], GL_STATIC_DRAW));

    GL(glEnableVertexAttribArray(0));
    GL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));

    GL(glGenBuffers(1, &m_cbo));
    GL(glBindBuffer(GL_ARRAY_BUFFER, m_cbo));
    GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, &coords[0], GL_STATIC_DRAW));

    GL(glEnableVertexAttribArray(1));
    GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0));

    GL(glBindVertexArray(0));
    GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Text::setString(const std::string& text)
{
    m_text = text;

    renderTexture();
    setupBuffers();
}

void Text::render() const
{
    GL(glBindVertexArray(m_vao));
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    GL(glBindVertexArray(0));
}

void Text::setOrigin(const vec2& origin)
{
    m_origin = origin;
}

vec2 Text::getOrigin() const
{
    return m_origin;
}

GLuint Text::getWidth() const
{
    return m_width;
}

GLuint Text::getHeight() const
{
    return m_height;
}

GLuint Text::getTexture() const
{
    return m_texture;
}
