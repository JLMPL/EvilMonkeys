#include "Frustum.hpp"

void Frustum::init(float fov, float aspect, float near, float far, const mat4& view)
{
    vec3 dir = vec3(0.f, 0.f, -1.f);

    float farHeight = 2 * tan(fov / 2) * far;
    float farWidth = farHeight * aspect;

    float nearHeight = 2 * tan(fov / 2) * near;
    float nearWidth = nearHeight * aspect;

    m_ftl = dir * far;
    m_ftl += vec3(-farWidth/2, farHeight/2, 0.f);

    m_ftr = m_ftl + vec3(farWidth, 0.f, 0.f);
    m_fbl = m_ftl + vec3(0.f, -farHeight, 0.f);
    m_fbr = m_fbl + vec3(farWidth, 0.f, 0.f);

    m_ntl = dir * near;
    m_ntl += vec3(-nearWidth/2, nearHeight/2, 0.f);

    m_ntr = m_ntl + vec3(nearWidth, 0.f, 0.f);
    m_nbl = m_ntl + vec3(0.f, -nearHeight, 0.f);
    m_nbr = m_nbl + vec3(nearWidth, 0.f, 0.f);

    update(view);
}

void Frustum::update(const mat4& view)
{
    mat4 iview = math::inverse(view);
    mat3 rot = mat3(iview);
    vec3 mov = vec3(iview[3]);

    m_tftl = (rot * m_ftl) + mov;
    m_tftr = (rot * m_ftr) + mov;
    m_tfbl = (rot * m_fbl) + mov;
    m_tfbr = (rot * m_fbr) + mov;

    m_tntl = (rot * m_ntl) + mov;
    m_tntr = (rot * m_ntr) + mov;
    m_tnbl = (rot * m_nbl) + mov;
    m_tnbr = (rot * m_nbr) + mov;
}

static bool rayPlane(
    const vec3& coord, const vec3& normal,
    const vec3& rayOrigin, const vec3& ray,
    vec3& contact
)
{
    float d = math::dot(normal, coord);

    if (math::dot(normal, ray) == 0.f)
        return false;

    float x = (d - math::dot(normal, rayOrigin)) / math::dot(normal, ray);

    contact = rayOrigin + math::normalize(ray)*x;
    return true;
}

static float distanceFromPlane(Frustum* fr, const vec3& point, const vec3& normal, const vec3& pos)
{
    vec3 contact;

    if (rayPlane(point, normal, pos, -normal, contact))
    {
        // fr->renderer->addLine(pos, contact, vec3(0.f,0.f,1.f));

        vec3 fus = pos - contact;

        if (math::dot(math::normalize(fus), normal) > 0)
        {
            return math::length(fus);
        }
        else
            return -5.f;
    }

    return -5.f;
}

bool Frustum::isInFrustum(const vec3& pos, float radius)
{
    //RIGHT
    vec3 ledges0[] = {
        m_tntr - m_tnbr,
        m_tntr - m_tftr
    };

    vec3 normal = math::normalize(math::cross(ledges0[0], ledges0[1]));

    if (distanceFromPlane(this, m_tntr, normal, pos) > radius)
    {
        return false; //CULL THIS BITCH
    }

    //LEFT
    vec3 ledges1[] = {
        m_tftl - m_tfbl,
        m_tftl - m_tntl
    };

    normal = math::normalize(math::cross(ledges1[0], ledges1[1]));

    if (distanceFromPlane(this, m_tftl, normal, pos) > radius)
    {
        return false; //CULL THIS BITCH
    }

    //TOP
    vec3 ledges2[] = {
        m_tftr - m_tftl, // |
        m_tftr - m_tntr  // --
    };

    normal = math::normalize(math::cross(ledges2[0], ledges2[1]));

    if (distanceFromPlane(this, m_tftr, normal, pos) > radius)
    {
        return false; //CULL THIS BITCH
    }

    //BOTTOM
    vec3 ledges3[] = {
        m_tfbl - m_tfbr, // |
        m_tfbl - m_tnbl  // --
    };

    normal = math::normalize(math::cross(ledges3[0], ledges3[1]));

    if (distanceFromPlane(this, m_tfbl, normal, pos) > radius)
    {
        return false; //CULL THIS BITCH
    }

    //FAR
    vec3 ledges4[] = {
        m_tftr - m_tfbr, // |
        m_tftr - m_tftl  // --
    };

    normal = math::normalize(math::cross(ledges4[0], ledges4[1]));

    if (distanceFromPlane(this, m_tftr, normal, pos) > radius)
    {
        return false; //CULL THIS BITCH
    }

    //NEAR
    vec3 ledges5[] = {
        m_tntl - m_tnbl, // |
        m_tntl - m_tntr  // --
    };

    normal = math::normalize(math::cross(ledges5[0], ledges5[1]));

    if (distanceFromPlane(this, m_tntl, normal, pos) > radius)
    {
        return false; //CULL THIS BITCH
    }

    return true; //HOMIE IS COOL
}

//SUNSHINE AND RAINBOWS
