#pragma once
#include "VertexArray.hpp"
#include "ArrayBuffer.hpp"

struct ScreenQuad
{
    VertexArray vao;
    ArrayBuffer vbo;
    ArrayBuffer tbo;

    void init();
    void render() const;
};
