#include "Skybox.hpp"
#include "Core/Math3d.hpp"

void Skybox::init(const char* name)
{
    std::string files[] = {std::string(name)};
    m_cubemap.initFromFiles(files, true);

    float size = 1000;

    vec3 verts[] =
    {
        vec3(size,-size,-size),
        vec3(size,-size,size),
        vec3(size,size,size),

        vec3(size,-size,-size),
        vec3(size,size,size),
        vec3(size,size,-size),

        vec3(-size,-size,-size),
        vec3(size,-size,-size),
        vec3(size,size,-size),

        vec3(-size,-size,-size),
        vec3(size,size,-size),
        vec3(-size,size,-size),

        vec3(-size, -size, size),
        vec3(-size, -size, -size),
        vec3(-size, size, -size),

        vec3(-size, -size, size),
        vec3(-size, size, -size),
        vec3(-size, size, size),

        vec3(size,-size,size),
        vec3(-size,-size,size),
        vec3(-size,size,size),

        vec3(size,-size,size),
        vec3(-size,size,size),
        vec3(size,size,size),

        //TOP
        vec3(-size,size,-size),
        vec3(size,size,-size),
        vec3(-size,size,size),

        vec3(size,size,-size),
        vec3(size,size,size),
        vec3(-size,size,size),

        //BOTTOM
        vec3(-size,-size,size),
        vec3(size,-size,-size),
        vec3(-size,-size,-size),

        vec3(-size,-size,size),
        vec3(size,-size,size),
        vec3(size,-size,-size)
    };

    m_vao.init();
    m_vao.bind();

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(float) * 3 * 36, &verts[0], GL_STATIC_DRAW);

    m_vao.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_vao.unbind();
    m_vbo.unbind();
}

void Skybox::render(bool bindCubemap) const
{
    if (bindCubemap)
        m_cubemap.bind();

    m_vao.bind();
    GL(glDrawArrays(GL_TRIANGLES, 0, 36));
    m_vao.unbind();

    if (bindCubemap)
        m_cubemap.unbind();
}

const Cubemap& Skybox::getCubemap() const
{
    return m_cubemap;
}
