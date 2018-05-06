#pragma once
#include "Core/Math3d.hpp"

class DebugRenderer;

struct Frustum
{
    Frustum() = default;

    void init(float fov, float aspect, float near, float far, const mat4& view);
    void update(const mat4& view);

    bool isInFrustum(const vec3& pos, float radius);

    DebugRenderer* renderer;

    vec3 m_ftl;
    vec3 m_ftr;
    vec3 m_fbl;
    vec3 m_fbr;

    vec3 m_ntl;
    vec3 m_ntr;
    vec3 m_nbl;
    vec3 m_nbr;

    vec3 m_tftl;
    vec3 m_tftr;
    vec3 m_tfbl;
    vec3 m_tfbr;

    vec3 m_tntl;
    vec3 m_tntr;
    vec3 m_tnbl;
    vec3 m_tnbr;
};
