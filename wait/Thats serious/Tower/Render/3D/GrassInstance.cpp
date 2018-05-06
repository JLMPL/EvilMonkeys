#include "GrassInstance.hpp"
#include "Core/Random.hpp"
#include <vector>

void GrassInstance::init(const vec3& pos)
{
    std::vector<vec3> verts;
    std::vector<vec3> normals;
    std::vector<vec4> colors;
    std::vector<vec2> coords;

    vec3 tuft[6];

    float width = Random::inRange<float>(1.f,1.5f);
    float height = Random::inRange<float>(2.f,3.f);

    tuft[0] = vec3(width, 0, 0);
    tuft[1] = vec3(width, height, 0);
    tuft[2] = vec3(-width, 0, 0);

    tuft[3] = vec3(-width, 0, 0);
    tuft[4] = vec3(width, height, 0);
    tuft[5] = vec3(-width, height, 0);

    for (auto i = 0; i < 30; i++)
    {
        float x = Random::inRange<float>(-5.f, 5.f);
        float y = Random::inRange<float>(0.f, M_TAU);
        float z = Random::inRange<float>(-5.f, 5.f);

        x *= 0.7f;
        z *= 0.7f;

        mat4 mat = math::translate(vec3(x,0,z)) * math::rotate(y, vec3(0,1,0));

        for (auto j = 0; j < 6; j++)
        {
            verts.push_back(vec3(mat * vec4(tuft[j], 1)));
        }

        vec3 normal = vec3(0,0,1) * mat3(math::rotate(y, vec3(0,1,0)));

        normals.push_back(normal);
        normals.push_back(vec3(0,1,0));
        normals.push_back(normal);

        normals.push_back(normal);
        normals.push_back(vec3(0,1,0));
        normals.push_back(vec3(0,1,0));

        float fucktor = Random::inRange<float>(0.f, 0.5f) + 0.5f;
        float offset = Random::inRange<float>(0.f, M_TAU);

        colors.push_back(vec4(0,0,0,1));
        colors.push_back(vec4(fucktor, offset, 0, 1));
        colors.push_back(vec4(0,0,0,1));

        colors.push_back(vec4(0,0,0,1));
        colors.push_back(vec4(fucktor, offset, 0, 1));
        colors.push_back(vec4(fucktor, offset, 0, 1));


        coords.push_back(vec2(1,1));
        coords.push_back(vec2(1,0));
        coords.push_back(vec2(0,1));

        coords.push_back(vec2(0,1));
        coords.push_back(vec2(1,0));
        coords.push_back(vec2(0,0));
    }

    m_vao.init();
    m_vao.bind();

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(float) * 3 * verts.size(), &verts[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_nbo.init(GL_ARRAY_BUFFER);
    m_nbo.bind();
    m_nbo.setData(sizeof(float) * 3 * verts.size(), &normals[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_cbo.init(GL_ARRAY_BUFFER);
    m_cbo.bind();
    m_cbo.setData(sizeof(float) * 4 * verts.size(), &colors[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

    m_tbo.init(GL_ARRAY_BUFFER);
    m_tbo.bind();
    m_tbo.setData(sizeof(float) * 2 * verts.size(), &coords[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_vao.unbind();
    m_vbo.unbind();
}

void GrassInstance::render() const
{
    m_vao.bind();
    GL(glDrawArrays(GL_TRIANGLES, 0, 6 * 30));
    m_vao.unbind();
}
