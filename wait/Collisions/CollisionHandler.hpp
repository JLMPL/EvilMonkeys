#ifndef COLLISION_HANDLER_HPP
#define COLLISION_HANDLER_HPP
#include "Collision.hpp"
#include "Shapes.hpp"
#include "Render/Renderer.hpp"
#include <vector>

struct RayCastResult
{
    bool hasHit = false;
    glm::vec3 point;
    glm::vec3 normal;
};

class CollisionHandler
{
    public:
        CollisionHandler() = default;
        CollisionHandler(const CollisionHandler&) = delete;
        CollisionHandler operator=(const CollisionHandler&) = delete;

        void init(Renderer* renderer = nullptr);
        void addTriangularShape(TriangularShape* shape);
        void addSphereShape(SphereShape* shape);
        void addEllipsoidShape(EllipsoidShape* shape);

        RayCastResult castRay(const glm::vec3& from, const glm::vec3& to);

        void resolveCollisions();

    private:
        struct ShapePair
        {
            CollisionShape* a;
            CollisionShape* b;

            ShapePair(CollisionShape* pa, CollisionShape* pb) : a(pa), b(pb) {}
        };

        void moveSphereFromPoint(SphereShape* shpere, const glm::vec3& along, float32 distFromPoint);
        void sphereTriangular(SphereShape* sphere, TriangularShape* trian);
        
        void ellipsoidTriangular(EllipsoidShape* elli, TriangularShape* trian);
        void ellipsoidTriangle(EllipsoidShape* elli, const CollisionTriangle& tri);
        void ellipsoidPoint(EllipsoidShape* elli, const glm::vec3& point);

        void doPair(const ShapePair& pair);
        void cachePairs();

        void renderAabb(const Aabb& aabb, const glm::vec3& color);
        void renderBounds();

    private:
        Renderer*                     m_renderer;

        std::vector<TriangularShape*> m_triangularShapes;
        std::vector<SphereShape*>     m_sphereShapes;
        std::vector<EllipsoidShape*>  m_ellipsoidShapes;

        std::vector<ShapePair>        m_pairs;
};

#endif
