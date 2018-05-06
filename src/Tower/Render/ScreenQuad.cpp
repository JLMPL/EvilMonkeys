#include "ScreenQuad.hpp"

void ScreenQuad::init()
{
    constexpr float verts[] = {
        1,-1,
        1,1,
        -1,1,
        1,-1,
        -1,1,
        -1,-1
    };

    constexpr float coords[] = {
        1,0,
        1,1,
        0,1,
        1,0,
        0,1,
        0,0
    };

    vao.init();
    vao.bind();

    vbo.init(GL_ARRAY_BUFFER);
    vbo.bind();
    vbo.setData(sizeof(float) * 12, &verts[0], GL_STATIC_DRAW);
    vao.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    tbo.init(GL_ARRAY_BUFFER);
    tbo.bind();
    tbo.setData(sizeof(float) * 12, &coords[0], GL_STATIC_DRAW);
    vao.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    vao.unbind();
    vbo.unbind();
}

void ScreenQuad::render() const
{
    vao.bind();
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    vao.unbind();
}
