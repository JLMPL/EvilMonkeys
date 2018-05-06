#include "MeshEntry.hpp"

void MeshEntry::create(
    const std::vector<vec3>& verts,
    const std::vector<vec3>& normals,
    const std::vector<vec4>& colors,
    const std::vector<vec2>& coords,
    const std::vector<MeshTriangle>& tris
    )
{
    m_numVertices = verts.size();
    m_numTriangles = verts.size()/3;

    m_vertices.resize(m_numVertices);
    m_normals.resize(m_numVertices);
    m_colors.resize(m_numVertices);
    m_coords.resize(m_numVertices);

    m_tangents.resize(m_numVertices);
    m_bitans.resize(m_numVertices);
    m_tris.resize(m_numTriangles);

    for (u32 i = 0; i < m_numVertices; i++)
    {
        m_vertices[i] = verts[i];
        m_normals[i] = normals[i];
        m_colors[i] = colors[i];
        m_coords[i] = coords[i];
    }

    for (u32 i = 0; i < m_numTriangles; i++)
    {
        m_tris[i] = tris[i];
    }

    calcTangents();
}

void MeshEntry::calcTangents()
{
    std::vector<vec3> tan1(m_numVertices);
    std::vector<vec3> tan2(m_numVertices);

    for (auto i = 0; i < m_numTriangles; i++)
    {
        u32 i1 = m_tris[i].inds[0];
        u32 i2 = m_tris[i].inds[1];
        u32 i3 = m_tris[i].inds[2];

        const vec3& v1 = m_vertices[i1];
        const vec3& v2 = m_vertices[i2];
        const vec3& v3 = m_vertices[i3];

        const vec2& w1 = m_coords[i1];
        const vec2& w2 = m_coords[i2];
        const vec2& w3 = m_coords[i3];

        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;

        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;

        float r = 1.f / (s1 * t2 - s2 * t1);

        vec3 sdir(
            (t2 * x1 - t1 * x2) * r,
            (t2 * y1 - t1 * y2) * r,
            (t2 * z1 - t1 * z2) * r
        );
        vec3 tdir(
            (s1 * x2 - s2 * x1) * r,
            (s1 * y2 - s2 * y1) * r,
            (s1 * z2 - s2 * z1) * r
        );

        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;

        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }

    for (auto i = 0; i < m_numVertices; i++)
    {
        const vec3& n = m_normals[i];
        const vec3& t = tan1[i];

        m_tangents[i] = math::normalize(t - n * math::dot(n, t));
        float w = (math::dot(math::cross(n, t), tan2[i]) < 0.f) ? -1.f : 1.f;

        m_bitans[i] = math::cross(n, m_tangents[i]) * w;
    }
}

void MeshEntry::setupBuffers()
{
    m_vao.init();
    m_vao.bind();

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(float) * 3 * m_numVertices, &m_vertices[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_nbo.init(GL_ARRAY_BUFFER);
    m_nbo.bind();
    m_nbo.setData(sizeof(float) * 3 * m_numVertices, &m_normals[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_cbo.init(GL_ARRAY_BUFFER);
    m_cbo.bind();
    m_cbo.setData(sizeof(float) * 4 * m_numVertices, &m_colors[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

    m_tbo.init(GL_ARRAY_BUFFER);
    m_tbo.bind();
    m_tbo.setData(sizeof(float) * 2 * m_numVertices, &m_coords[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_tansbo.init(GL_ARRAY_BUFFER);
    m_tansbo.bind();
    m_tansbo.setData(sizeof(float) * 3 * m_numVertices, &m_tangents[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_bitansbo.init(GL_ARRAY_BUFFER);
    m_bitansbo.bind();
    m_bitansbo.setData(sizeof(float) * 3 * m_numVertices, &m_bitans[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_ibo.init(GL_ELEMENT_ARRAY_BUFFER);
    m_ibo.bind();
    m_ibo.setData(sizeof(GLuint) * 3 * m_numTriangles, &m_tris[0], GL_STATIC_DRAW);

    m_vao.unbind();
    m_vbo.unbind();
    m_ibo.unbind();
}

void MeshEntry::render() const
{
    m_vao.bind();
    GL(glDrawElements(GL_TRIANGLES, m_numTriangles * 3, GL_UNSIGNED_INT, 0));
    m_vao.unbind();
}
