#pragma once
#include "../OpenGL.hpp"
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"
#include "Cubemap.hpp"

class Skybox
{
    public:
        Skybox() = default;
        ~Skybox() = default;

        void init(const char* name);
        void render(bool) const;

        const Cubemap& getCubemap() const;

    private:
        Cubemap        m_cubemap;

        VertexArray    m_vao;
        ArrayBuffer    m_vbo;
};
