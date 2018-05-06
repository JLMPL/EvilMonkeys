#pragma once
#include "Collision.hpp"
#include "Shapes/Sphere.hpp"
#include "Shapes/Ellipsoid.hpp"
#include "Shapes/Polyhedron.hpp"
#include <vector>

struct RayCastResult
{
    bool hasHit = false;
    vec3 point;
    vec3 normal;
};

class CollisionHandler
{
    public:
        CollisionHandler() = default;
        CollisionHandler(const CollisionHandler&) = delete;
        CollisionHandler operator=(const CollisionHandler&) = delete;

        void init();
        void addPolyhedron(Shape::Polyhedron* shape);
        void addSphere(Shape::Sphere* shape);
        void addEllipsoid(Shape::Ellipsoid* shape);

        RayCastResult castRay(const vec3& from, const vec3& to);

        void resolveCollisions();

    private:
        struct ShapePair
        {
            Shape::BaseShape* a;
            Shape::BaseShape* b;
        };

        void moveSphereFromPoint(Shape::Sphere* shpere, const vec3& along, f32 distFromPoint);
        void spherePolyhedron(Shape::Sphere* sphere, Shape::Polyhedron* trian);

        void ellipsoidPolyhedron(Shape::Ellipsoid* elli, Shape::Polyhedron* trian);
        void ellipsoidTriangle(Shape::Ellipsoid* elli, const Shape::Triangle& tri);
        void ellipsoidPoint(Shape::Ellipsoid* elli, const vec3& point);

        void doPair(const ShapePair& pair);
        void cachePairs();

        void renderAabb(const Aabb& aabb, const vec3& color);
        void renderBounds();

    private:
        std::vector<Shape::Polyhedron*> m_polyhedra;
        std::vector<Shape::Sphere*>     m_spheres;
        std::vector<Shape::Ellipsoid*>  m_ellipsoids;

        std::vector<ShapePair> m_pairs;
};
