#include "Collision.hpp"
#include "Core/Types.hpp"
#include <stdio.h>

static bool rayPlane(
    const vec3& coord, const vec3& normal,
    const vec3& rayOrigin, const vec3& ray,
    vec3& contact
)
{
    f32 d = math::dot(normal, coord);

    if (math::dot(normal, ray) == 0)
        return false;

    f32 x = (d - math::dot(normal, rayOrigin)) / math::dot(normal, ray);

    contact = rayOrigin + math::normalize(ray)*x;
    return true;
}

static f32 triangleArea(const vec3& av, const vec3& bv, const vec3& cv)
{
    f32 a = math::length(av - bv);
    f32 b = math::length(bv - cv);
    f32 c = math::length(cv - av);

    f32 s = (a + b + c) / 2.f;

    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

bool rayTriangle(const Shape::Triangle& triangle, const vec3& from, const vec3& to, f32& out)
{
    vec3 hit;

    if (!rayPlane(triangle.verts[0], triangle.normal, from, to, hit))
        return false;

    f32 fullArea = triangleArea(triangle.verts[0], triangle.verts[1], triangle.verts[2]);
    f32 eax = triangleArea(triangle.verts[0], triangle.verts[1], hit);
    f32 ebx = triangleArea(triangle.verts[1], triangle.verts[2], hit);
    f32 ecx = triangleArea(triangle.verts[2], triangle.verts[0], hit);

    if (std::abs(fullArea - (eax + ebx + ecx)) > Epsilon)
        return false;

    out = math::length(from - hit);
    return true;
}

vec3 pointEdge(const vec3& a, const vec3& b, const vec3& point)
{
    vec3 diff = b - a;
    f32 sqrLength =
        diff.x * diff.x +
        diff.y * diff.y +
        diff.z * diff.z;

    vec3 lineToPointVec = point - a;
    f32 dot = math::dot(diff, lineToPointVec);

    f32 percentage = dot / sqrLength;

    if (percentage < 0.f || percentage > 1.f)
    {
        return vec3();
    }
    return vec3(a + ((b - a) * percentage));
}

bool aabbOverlapping(const Aabb& a, const Aabb& b)
{
    bool xy = true;
    bool xz = true;
    bool yz = true;

    f32 ax, ay, aw, ah;
    f32 bx, by, bw, bh;

    ax = a.min.x;
    ay = a.min.y;
    aw = a.max.x - ax;
    ah = a.max.y - ay;

    bx = b.min.x;
    by = b.min.y;
    bw = b.max.x - bx;
    bh = b.max.y - by;

    if (ax > bx + bw)
        xy = false;
    if (ay > by + bh)
        xy = false;
    if (ax + aw < bx)
        xy = false;
    if (ay + ah < by)
        xy = false;

    ay = a.min.z;
    ah = a.max.z - ay;

    by = b.min.z;
    bh = b.max.z - by;

    if (ax > bx + bw)
        xz = false;
    if (ay > by + bh)
        xz = false;
    if (ax + aw < bx)
        xz = false;
    if (ay + ah < by)
        xz = false;

    ax = a.min.y;
    aw = a.max.y - ax;

    bx = b.min.y;
    bw = b.max.y - bx;

    if (ax > bx + bw)
        yz = false;
    if (ay > by + bh)
        yz = false;
    if (ax + aw < bx)
        yz = false;
    if (ay + ah < by)
        yz = false;

    return (xy && xz && yz) ? true : false;
}
