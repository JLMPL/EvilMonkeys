#pragma once
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"
#include "Core/Math3d.hpp"
#include <string>

class Font;

class Text
{
    public:
        Text() = default;
        ~Text();

        void init(const Font* font);
        void setString(const std::string& str, f32 size = 1.f);
        void render() const;

        void setPosition(const vec2& vec);
        vec2 getPosition() const;

        u32 getWidth() const;
        u32 getHeight() const;
        vec2u getSize() const;

    private:
        const Font* m_font = nullptr;
        std::string m_string;

        f32 m_size = 1;

        VertexArray m_vao;
        ArrayBuffer m_vbo;
        ArrayBuffer m_tbo;

        u32 m_width = 0;
        u32 m_height = 0;

        vec2 m_pos;
};
