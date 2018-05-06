#include "Shapes.hpp"

void CollisionShape::move(const glm::vec3& offset)
{
    m_pos += offset;
}

Aabb CollisionShape::getAabb()
{
    return {m_aabb.min + m_pos, m_aabb.max + m_pos};
}

//SPHERE

SphereShape::SphereShape()
{
    m_type = CollisionShape::Type::Sphere;
}

SphereShape::SphereShape(float32 radius) : m_radius(radius)
{
    m_type = CollisionShape::Type::Sphere;
    constructAabb();
}

void SphereShape::constructAabb()
{
    m_aabb.checkPoint({m_radius, m_radius, m_radius});
    m_aabb.checkPoint({-m_radius, -m_radius, -m_radius});
}

//ELLIPSOID

EllipsoidShape::EllipsoidShape()
{
    m_type = CollisionShape::Type::Ellipsoid;
}

EllipsoidShape::EllipsoidShape(const glm::vec3& radii) : m_radii(radii)
{
    m_type = CollisionShape::Type::Ellipsoid;
    constructAabb();
}

void EllipsoidShape::constructAabb()
{
    m_aabb.checkPoint({m_radii.x, m_radii.y, m_radii.z});
    m_aabb.checkPoint({-m_radii.x, -m_radii.y, -m_radii.z});
}

//TRIANGLE

CollisionTriangle::CollisionTriangle(const vec3f& a, const vec3f& b, const vec3f& c, const vec3f& n)
{
    verts[0] = a;
    verts[1] = b;
    verts[2] = c;
    normal = n;
}

//TRIANGULAR

TriangularShape::TriangularShape()
{
    m_type = CollisionShape::Type::Triangular;
}

void TriangularShape::init()
{
    constructAabb();
}

void TriangularShape::constructAabb()
{
    for (uint32 i = 0; i < m_tris.size(); i++)
    {
        m_aabb.checkPoint(m_tris[i].verts[0]);
        m_aabb.checkPoint(m_tris[i].verts[1]);
        m_aabb.checkPoint(m_tris[i].verts[2]);
    }
}

void TriangularShape::addTriangle(const CollisionTriangle& tri)
{
    m_tris.push_back(tri);
}
