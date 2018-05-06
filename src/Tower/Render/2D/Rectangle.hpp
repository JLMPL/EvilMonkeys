#pragma once
#include "Core/Math3d.hpp"
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"
#include <vector>

//LET'S EMULATE SOME SFML HERE

class Rectangle
{
    public:
        Rectangle() = default;
        ~Rectangle() = default;

        void init(const vec2& size);

        void render() const;

        void setSize(const vec2& size);

        void setColor(const vec4& color);
        vec4 getColor() const;

        void setPosition(const vec2& pos);
        vec2 getPosition() const;

    private:
        void initBuffers();

    private:
        vec2 m_size;
        vec2 m_pos;
        vec4 m_color;

        VertexArray m_vao;
        ArrayBuffer m_vbo;
        ArrayBuffer m_tbo;
};
