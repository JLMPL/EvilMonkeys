#pragma once
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"
#include "Core/Math3d.hpp"

struct Material;

class GrassInstance //mass bass ass brass glass gas
{
    public:
        GrassInstance() = default;

        void init(const vec3& pos);
        void render() const;

    public:
        mat4 m_transform;
        const Material* m_material = nullptr;

    private:
        VertexArray m_vao;
        ArrayBuffer m_vbo;
        ArrayBuffer m_nbo;
        ArrayBuffer m_cbo;
        ArrayBuffer m_tbo;
};
