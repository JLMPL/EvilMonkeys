#ifndef SHAPES_HPP
#define SHAPES_HPP
#include "AxisAlignedBoundingBox.hpp"
#include "Core/Types.hpp"

class CollisionShape
{
    public:
        enum class Type
        {
            Sphere,
            Ellipsoid,
            Triangular
        };

        CollisionShape() = default;

        void move(const glm::vec3& offset)
        {
            m_pos += offset;
        }

        void setPosition(const glm::vec3& pos)
        {
            m_pos = pos;
        }
        
        const glm::vec3& getPosition()
        {
            return m_pos;
        }

        AxisAlignedBoundingBox getAabb()
        {
            return {m_aabb.min + m_pos, m_aabb.max + m_pos};
        }

        virtual Type getType() const = 0;

    private:
        virtual void constructAabb() {};

    protected:
        AxisAlignedBoundingBox m_aabb;
        glm::vec3 m_pos;

};

struct SphereShape : public CollisionShape
{
    public:
        SphereShape() = default;
        SphereShape(float32 radius) : m_radius(radius)
        {
            constructAabb();
        }

        float getRadius() const 
        {
            return m_radius;
        }

        Type getType() const override final { return Type::Sphere; }

    private:
        void constructAabb() override final
        {
            m_aabb.checkPoint({m_radius, m_radius, m_radius});
            m_aabb.checkPoint({-m_radius, -m_radius, -m_radius});
        }

    private:
        float m_radius = 1.f;
};

struct EllipsoidShape : public CollisionShape
{
    public:    
        EllipsoidShape() = default;
        EllipsoidShape(const glm::vec3& radii) : m_radii(radii)
        {
            constructAabb();
        }

        const glm::vec3& getRadii() const
        {
            return m_radii;
        }

        Type getType() const override final { return Type::Ellipsoid; }

    private:
        void constructAabb() override final
        {
            m_aabb.checkPoint({m_radii.x, m_radii.y, m_radii.z});
            m_aabb.checkPoint({-m_radii.x, -m_radii.y, -m_radii.z});
        }

    private:
        glm::vec3 m_radii;
};

struct CollisionTriangle
{
    glm::vec3 verts[3];
    glm::vec3 normal;
};

struct TriangularShape : public CollisionShape
{
    public:
        TriangularShape() = default;
        TriangularShape(uint32 numTris) : m_numTris(numTris)
        {
            m_tris = new CollisionTriangle[m_numTris];
        }

        ~TriangularShape()
        {
            if (m_tris) delete[] m_tris;
        }

        void init()
        {
            constructAabb();
        }

        uint32 getNumTris() const
        {
            return m_numTris;
        }

        const CollisionTriangle& getTriangle(uint32 index) const
        {
            return m_tris[index];
        }

        Type getType() const override final { return Type::Triangular; }

    private:
        void constructAabb() override final
        {
            for (uint32 i = 0; i < m_numTris; i++)
            {
                m_aabb.checkPoint(m_tris[i].verts[0]);
                m_aabb.checkPoint(m_tris[i].verts[1]);
                m_aabb.checkPoint(m_tris[i].verts[2]);
            }
        }

    private:
        CollisionTriangle* m_tris = nullptr;
        uint32 m_numTris = 0;
};

#endif
