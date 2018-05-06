#include "Collision.hpp"
#include "Core/Types.hpp"
#include <stdio.h>

static bool rayPlane(
    const glm::vec3& coord, const glm::vec3& normal,
    const glm::vec3& rayOrigin, const glm::vec3& ray,
    glm::vec3& contact
)
{
    float32 d = glm::dot(normal, coord); //got it

    if (glm::dot(normal, ray) == 0)
        return false;

    // Compute the X value for the directed line ray intersecting the plane
    float32 x = (d - glm::dot(normal, rayOrigin)) / glm::dot(normal, ray);

    contact = rayOrigin + glm::normalize(ray)*x; //got it
    return true;
}

static float32 triangleArea(const glm::vec3& av, const glm::vec3& bv, const glm::vec3& cv)
{
    float32 a = glm::length(av - bv);
    float32 b = glm::length(bv - cv);
    float32 c = glm::length(cv - av);

    float32 s = (a + b + c) / 2.f;

    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

bool rayTriangle(const CollisionTriangle& triangle, const glm::vec3& from, const glm::vec3& to, float32& out)
{
    glm::vec3 hit;

    if (!rayPlane(triangle.verts[0], triangle.normal, from, to, hit))
        return false;

    float32 fullArea = triangleArea(triangle.verts[0], triangle.verts[1], triangle.verts[2]);
    float32 eax = triangleArea(triangle.verts[0], triangle.verts[1], hit);
    float32 ebx = triangleArea(triangle.verts[1], triangle.verts[2], hit);
    float32 ecx = triangleArea(triangle.verts[2], triangle.verts[0], hit);

    if (std::abs(fullArea - (eax + ebx + ecx)) > Epsilon)
        return false;

    out = glm::length(from - hit);
    return true;
}

glm::vec3 pointEdge(const glm::vec3& a, const glm::vec3& b, const glm::vec3& point)
{
    glm::vec3 diff = b - a;
    float32 sqrLength = //glm::length2(diff);
        diff.x * diff.x +
        diff.y * diff.y +
        diff.z * diff.z;

    glm::vec3 lineToPointVec = point - a;
    float32 dot = glm::dot(diff, lineToPointVec);

    float32 percentage = dot / sqrLength;

    if(percentage < 0.f or percentage > 1.f)
    {
        return glm::vec3();
    }
    return glm::vec3(a + ((b - a) * percentage));
}

bool aabbOverlapping(const Aabb& a, const Aabb& b)
{
    bool xy = true;
    bool xz = true;
    bool yz = true;

    float32 ax, ay, aw, ah;
    float32 bx, by, bw, bh;

    //xy

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

    //xz

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

    //yz

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
