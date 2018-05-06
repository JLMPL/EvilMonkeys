#pragma once
#include "Font.hpp"
#include "OpenGL.hpp"
#include "VertexArray.hpp"
#include "ArrayBuffer.hpp"
#include "Core/Math3d.hpp"
#include <SDL2/SDL_ttf.h>
#include <string>

class Text
{
    public:
        Text() = default;
        ~Text();

        void init(const Font* font);
        void setString(const std::string& text);
        void render() const;

        void setOrigin(const vec2& origin);
        vec2 getOrigin() const;

        GLuint getWidth() const;
        GLuint getHeight() const;

        GLuint getTexture() const;

    private:
        void renderTexture();
        void setupBuffers();

    private:
        const Font*    m_font;
        std::string    m_text;

        u32         m_width = 0;
        u32         m_height = 0;

        vec2          m_origin;

        GLuint         m_texture = 0;

        GLuint         m_vao = 0;
        GLuint         m_vbo = 0;
        GLuint         m_cbo = 0;
};
