#pragma once
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"

class Cube
{
    public:
        Cube() = default;
        ~Cube() = default;

        void init(float size);
        void render() const;

    private:
        VertexArray m_vao;
        ArrayBuffer m_vbo;
};
