#ifndef SHAPES_HPP
#define SHAPES_HPP
#include "Aabb.hpp"
#include "Core/Math3d.hpp"
#include <vector>

struct CollisionShape
{
    enum class Type
    {
        Sphere,
        Ellipsoid,
        Triangular
    };

    void move(const glm::vec3& offset);
    Aabb getAabb();

    virtual void constructAabb() = 0;

    Type         m_type;
    Aabb         m_aabb;
    glm::vec3    m_pos;

};

struct SphereShape : CollisionShape
{
    SphereShape();
    SphereShape(float32 radius);

    void constructAabb() override final;

    float m_radius = 1.f;
};

struct EllipsoidShape : CollisionShape
{
    EllipsoidShape();
    EllipsoidShape(const glm::vec3& radii);

    void constructAabb() override final;

    glm::vec3 m_radii;
};

struct CollisionTriangle
{
    glm::vec3 verts[3];
    glm::vec3 normal;

    CollisionTriangle(const vec3f& a, const vec3f& b, const vec3f& c, const vec3f& n);
};

struct TriangularShape : CollisionShape
{
    TriangularShape();

    void init();
    void constructAabb() override final;

    void addTriangle(const CollisionTriangle& tri);

    std::vector<CollisionTriangle> m_tris;
};

#endif
