#include "CollisionHandler.hpp"
#include <algorithm>
#include <glm/ext.hpp>

void CollisionHandler::init()
{
}

void CollisionHandler::addSphere(Shape::Sphere* shape)
{
    m_spheres.push_back(shape);
}

void CollisionHandler::addEllipsoid(Shape::Ellipsoid* shape)
{
    m_ellipsoids.push_back(shape);
}

void CollisionHandler::addPolyhedron(Shape::Polyhedron* shape)
{
    m_polyhedra.push_back(shape);
}

RayCastResult CollisionHandler::castRay(const vec3& from, const vec3& to)
{
    Aabb rayAabb;
    rayAabb.checkPoint(from);
    rayAabb.checkPoint(to + 100.f);

    rayAabb.max += vec3(0.1f);
    rayAabb.min -= vec3(0.1f);

    std::vector<const Shape::Polyhedron*> potentials;

    for (auto& i : m_polyhedra)
    {
        if (aabbOverlapping(rayAabb, i->getAabb()))
        {
            potentials.push_back(i);
        }
    }

    if (potentials.empty())
        return {false, {0.f,0.f,0.f}, {0.f,1.f,0.f}};

    std::vector<vec3> points;
    std::vector<vec3> norms;

    for (auto& pot : potentials)
    {
        for (auto i = 0; i < pot->triangles.size(); i++)
        {
            f32 dist;
            if (rayTriangle(pot->triangles[i], from, to, dist))
            {
                points.push_back(from + to * dist);
                norms.push_back(pot->triangles[i].normal);
            }
        }
    }

    i32 closest = -1;
    f32 diste = std::numeric_limits<f32>::infinity();

    if (points.empty())
        return {false, {0.f,0.f,0.f}, {0.f,1.f,0.f}};

    for (auto i = 0; i < points.size(); i++)
    {
        vec3 tmp = from - points[i];
        f32 sqrdist = tmp.x * tmp.x + tmp.y * tmp.y;

        if (sqrdist < diste)
        {
            closest = i;
            diste = sqrdist;
        }
    }

    if (closest == -1)
        return {false, vec3(0), math::vecY};

    return {true, points[closest], norms[closest]};
}

void CollisionHandler::moveSphereFromPoint(Shape::Sphere* sphere, const vec3& along, f32 distFromPoint)
{
    if (distFromPoint < sphere->radius)
    {
        f32 diff = sphere->radius - distFromPoint;
        sphere->move(along * diff);
    }
}

void CollisionHandler::spherePolyhedron(Shape::Sphere* sphere, Shape::Polyhedron* trian)
{
    for (u32 i = 0; i < trian->triangles.size(); i++)
    {
        f32 dist = 0;

        if (rayTriangle(trian->triangles[i], sphere->pos, -trian->triangles[i].normal, dist))
        {
            moveSphereFromPoint(sphere, trian->triangles[i].normal, dist);
        }

        vec3 points[6] = {
            pointEdge(trian->triangles[i].verts[0], trian->triangles[i].verts[1], sphere->pos),
            pointEdge(trian->triangles[i].verts[1], trian->triangles[i].verts[2], sphere->pos),
            pointEdge(trian->triangles[i].verts[2], trian->triangles[i].verts[0], sphere->pos),
            trian->triangles[i].verts[0],
            trian->triangles[i].verts[1],
            trian->triangles[i].verts[2]
        };

        for (u32 i = 0; i < 6; i++)
        {
            f32 diste = math::length(points[i] - sphere->pos);
            moveSphereFromPoint(sphere, sphere->pos - points[i], diste);
        }
    }
}

void CollisionHandler::ellipsoidPoint(Shape::Ellipsoid* elli, const vec3& point)
{
    vec3 vec = point - elli->pos;
    vec3 radius = math::normalize(vec);
    radius *= elli->radii;

    f32 distance = math::length(vec);

    if (distance < math::length(radius))
    {
        f32 diff = math::length(radius) - distance;
        elli->move(-math::normalize(vec) * diff);
    }
}

void CollisionHandler::ellipsoidTriangle(Shape::Ellipsoid* elli, const Shape::Triangle& tri)
{
    f32 dist = 0;

    if (rayTriangle(tri, elli->pos, -tri.normal, dist))
    {
        ellipsoidPoint(elli, elli->pos - tri.normal * dist);
    }

    vec3 points[6] = {
        pointEdge(tri.verts[0], tri.verts[1], elli->pos),
        pointEdge(tri.verts[1], tri.verts[2], elli->pos),
        pointEdge(tri.verts[2], tri.verts[0], elli->pos),
        tri.verts[0],
        tri.verts[1],
        tri.verts[2]
    };

    for (u32 i = 0; i < 6; i++)
    {
        ellipsoidPoint(elli, points[i]);
    }
}

void CollisionHandler::ellipsoidPolyhedron(Shape::Ellipsoid* elli, Shape::Polyhedron* trian)
{
    for (u32 i = 0; i < trian->triangles.size(); i++)
    {
        ellipsoidTriangle(elli, trian->triangles[i]);
    }
}

void CollisionHandler::doPair(const ShapePair& pair)
{
    if (pair.a->type == Shape::BaseShape::Type::Sphere &&
        pair.b->type == Shape::BaseShape::Type::Polyhedron)
        spherePolyhedron(static_cast<Shape::Sphere*>(pair.a), static_cast<Shape::Polyhedron*>(pair.b));

    else if (pair.a->type == Shape::BaseShape::Type::Polyhedron &&
             pair.b->type == Shape::BaseShape::Type::Sphere)
        spherePolyhedron(static_cast<Shape::Sphere*>(pair.b), static_cast<Shape::Polyhedron*>(pair.a));

    else if (pair.a->type == Shape::BaseShape::Type::Ellipsoid &&
             pair.b->type == Shape::BaseShape::Type::Polyhedron)
        ellipsoidPolyhedron(static_cast<Shape::Ellipsoid*>(pair.a), static_cast<Shape::Polyhedron*>(pair.b));

    else if (pair.a->type == Shape::BaseShape::Type::Polyhedron &&
             pair.b->type == Shape::BaseShape::Type::Ellipsoid)
        ellipsoidPolyhedron(static_cast<Shape::Ellipsoid*>(pair.b), static_cast<Shape::Polyhedron*>(pair.a));
    else
        return;
}

void CollisionHandler::renderAabb(const Aabb& aabb, const vec3& color)
{
    const vec3& min = aabb.min;
    const vec3& max = aabb.max;

    vec3 a(min.x, max.y, min.z);
    vec3 b(max.x, max.y, min.z);
    vec3 c(min.x, min.y, min.z);
    vec3 d(max.x, min.y, min.z);
    vec3 e(min.x, max.y, max.z);
    vec3 f(max.x, max.y, max.z);
    vec3 g(min.x, min.y, max.z);
    vec3 h(max.x, min.y, max.z);
}

void CollisionHandler::renderBounds()
{
    /*
    for (auto& ts : m_Shape::Polyhedrons)
    {
        renderAabb(ts->getAabb(), vec3(1.f,1.f,1.f));

        if (ts)
        {
            for (auto i = 0; i < ts->triangles.size(); i++)
            {
                m_renderer->m_debugRenderer.addLine(ts->triangles[i].verts[0], ts->triangles[i].verts[1], vec3f(1.f));
                m_renderer->m_debugRenderer.addLine(ts->triangles[i].verts[1], ts->triangles[i].verts[2], vec3f(1.f));
                m_renderer->m_debugRenderer.addLine(ts->triangles[i].verts[2], ts->triangles[i].verts[0], vec3f(1.f));
            }
        }
    }

    for (auto& ss : m_spheres)
    {
        m_renderer->m_debugRenderer.addAxis(ss->pos);
        renderAabb(ss->getAabb(), vec3(1.f,1.f,1.f));
    }

    for (auto& es : m_ellipsoids)
        renderAabb(es->getAabb(), vec3(1.f,1.f,1.f));
    */
}

void CollisionHandler::cachePairs()
{
    for (auto& tri : m_polyhedra)
    for (auto& sph : m_spheres)
    {
        if (aabbOverlapping(tri->getAabb(), sph->getAabb()))
        {
            m_pairs.push_back({tri, sph});
        }
    }

    for (auto& tri : m_polyhedra)
    for (auto& eli : m_ellipsoids)
    {
        if (aabbOverlapping(tri->getAabb(), eli->getAabb()))
        {
            m_pairs.push_back({tri, eli});
        }
    }
}

void CollisionHandler::resolveCollisions()
{
    cachePairs();

    for (auto& pair : m_pairs)
        doPair(pair);

    m_pairs.clear();

    // if (m_renderer)
    //     renderBounds();
}
