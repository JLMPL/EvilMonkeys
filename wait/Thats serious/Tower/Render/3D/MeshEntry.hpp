#pragma once
#include "../OpenGL.hpp"
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"
#include "Core/Math3d.hpp"
#include <string>
#include <vector>

class Texture;

struct MeshTriangle
{
    GLuint inds[3] = {0,0,0};
};

struct MeshEntry
{
    public:
        void create(
            const std::vector<vec3>& verts,
            const std::vector<vec3>& normals,
            const std::vector<vec4>& colors,
            const std::vector<vec2>& coords,
            const std::vector<MeshTriangle>& tris
        );

        void calcTangents();
        void setupBuffers();

        void render() const;

    public:
        std::vector<vec3>            m_vertices;
        std::vector<vec3>            m_normals;
        std::vector<vec4>            m_colors;
        std::vector<vec2>            m_coords;
        std::vector<vec3>            m_tangents;
        std::vector<vec3>            m_bitans;
        std::vector<MeshTriangle>    m_tris;

        u32                          m_numVertices = 0;
        u32                          m_numTriangles = 0;

        VertexArray                  m_vao;
        ArrayBuffer                  m_vbo;
        ArrayBuffer                  m_nbo;
        ArrayBuffer                  m_cbo;
        ArrayBuffer                  m_tbo;
        ArrayBuffer                  m_tansbo;
        ArrayBuffer                  m_bitansbo;
        ArrayBuffer                  m_ibo;

    friend class SceneRenderer;
    friend class Mesh;
};
