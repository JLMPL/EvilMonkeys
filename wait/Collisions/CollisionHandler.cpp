#include "CollisionHandler.hpp"
#include <algorithm>
#include <glm/ext.hpp>

void CollisionHandler::init(Renderer* renderer)
{
    m_renderer = renderer;
}

void CollisionHandler::addTriangularShape(TriangularShape* shape)
{
    m_triangularShapes.push_back(shape);
}

void CollisionHandler::addSphereShape(SphereShape* shape)
{
    m_sphereShapes.push_back(shape);
}

void CollisionHandler::addEllipsoidShape(EllipsoidShape* shape)
{
    m_ellipsoidShapes.push_back(shape);
}

RayCastResult CollisionHandler::castRay(const glm::vec3& from, const glm::vec3& to)
{
    Aabb rayAabb;
    rayAabb.checkPoint(from);
    rayAabb.checkPoint(to + 20.f);

    rayAabb.max += glm::vec3(0.1f);
    rayAabb.min -= glm::vec3(0.1f);

    std::vector<const TriangularShape*> potentials;

    for (auto& i : m_triangularShapes)
    {
        if (aabbOverlapping(rayAabb, i->getAabb()))
        {
            potentials.push_back(i);
        }
    }

    if (potentials.empty())
        return {false, {0.f,0.f,0.f}, {0.f,1.f,0.f}};

    std::vector<glm::vec3> points;
    std::vector<glm::vec3> norms;

    for (auto& pot : potentials)
    {
        for (auto i = 0; i < pot->m_tris.size(); i++)
        {
            float dist;
            if (rayTriangle(pot->m_tris[i], from, to, dist))
            {
                points.push_back(from + to * dist);
                norms.push_back(pot->m_tris[i].normal);
            }
        }
    }

    int32 closest = -1;
    float diste = std::numeric_limits<float>::infinity();

    if (points.empty())
        return {false, {0.f,0.f,0.f}, {0.f,1.f,0.f}};

    for (auto i = 0; i < points.size(); i++)
    {
        glm::vec3 tmp = from - points[i];
        float sqrdist = tmp.x * tmp.x + tmp.y * tmp.y;

        if (sqrdist < diste)
        {
            closest = i;
            diste = sqrdist;
        }
    }

    if (closest == -1)
        return {false, {0.f,0.f,0.f}, {0.f,1.f,0.f}};

    return {true, points[closest], norms[closest]};
}

void CollisionHandler::moveSphereFromPoint(SphereShape* sphere, const glm::vec3& along, float32 distFromPoint)
{
    if (distFromPoint < sphere->m_radius)
    {
        float32 diff = sphere->m_radius - distFromPoint;
        sphere->move(along * diff);
    }
}

void CollisionHandler::sphereTriangular(SphereShape* sphere, TriangularShape* trian)
{
    for (uint32 i = 0; i < trian->m_tris.size(); i++)
    {
        float32 dist = 0;

        if (rayTriangle(trian->m_tris[i], sphere->m_pos, -trian->m_tris[i].normal, dist))
        {
            moveSphereFromPoint(sphere, trian->m_tris[i].normal, dist);
        }

        glm::vec3 points[6] = {
            pointEdge(trian->m_tris[i].verts[0], trian->m_tris[i].verts[1], sphere->m_pos),
            pointEdge(trian->m_tris[i].verts[1], trian->m_tris[i].verts[2], sphere->m_pos),
            pointEdge(trian->m_tris[i].verts[2], trian->m_tris[i].verts[0], sphere->m_pos),
            trian->m_tris[i].verts[0],
            trian->m_tris[i].verts[1],
            trian->m_tris[i].verts[2]
        };

        for (uint32 i = 0; i < 6; i++)
        {
            float32 diste = glm::length(points[i] - sphere->m_pos);
            moveSphereFromPoint(sphere, sphere->m_pos - points[i], diste);
        }
    }
}

void CollisionHandler::ellipsoidPoint(EllipsoidShape* elli, const glm::vec3& point)
{
    glm::vec3 vec = point - elli->m_pos;
    glm::vec3 radius = glm::normalize(vec);
    radius *= elli->m_radii;

    float32 distance = glm::length(vec);

    if (distance < glm::length(radius))
    {
        float diff = glm::length(radius) - distance;
        elli->move(-glm::normalize(vec) * diff);
    }
}

void CollisionHandler::ellipsoidTriangle(EllipsoidShape* elli, const CollisionTriangle& tri)
{
    float32 dist = 0;

    if (rayTriangle(tri, elli->m_pos, -tri.normal, dist))
    {
        ellipsoidPoint(elli, elli->m_pos - tri.normal * dist);
    }

    glm::vec3 points[6] = {
        pointEdge(tri.verts[0], tri.verts[1], elli->m_pos),
        pointEdge(tri.verts[1], tri.verts[2], elli->m_pos),
        pointEdge(tri.verts[2], tri.verts[0], elli->m_pos),
        tri.verts[0],
        tri.verts[1],
        tri.verts[2]
    };

    for (uint32 i = 0; i < 6; i++)
    {
        ellipsoidPoint(elli, points[i]);
    }
}

void CollisionHandler::ellipsoidTriangular(EllipsoidShape* elli, TriangularShape* trian)
{
    for (uint32 i = 0; i < trian->m_tris.size(); i++)
    {
        ellipsoidTriangle(elli, trian->m_tris[i]);
    }
}

void CollisionHandler::doPair(const ShapePair& pair)
{
    if (pair.a->m_type == CollisionShape::Type::Sphere &&
        pair.b->m_type == CollisionShape::Type::Triangular)
        sphereTriangular(static_cast<SphereShape*>(pair.a), static_cast<TriangularShape*>(pair.b));

    else if (pair.a->m_type == CollisionShape::Type::Triangular &&
             pair.b->m_type == CollisionShape::Type::Sphere)
        sphereTriangular(static_cast<SphereShape*>(pair.b), static_cast<TriangularShape*>(pair.a));

    else if (pair.a->m_type == CollisionShape::Type::Ellipsoid &&
             pair.b->m_type == CollisionShape::Type::Triangular)
        ellipsoidTriangular(static_cast<EllipsoidShape*>(pair.a), static_cast<TriangularShape*>(pair.b));

    else if (pair.a->m_type == CollisionShape::Type::Triangular &&
             pair.b->m_type == CollisionShape::Type::Ellipsoid)
        ellipsoidTriangular(static_cast<EllipsoidShape*>(pair.b), static_cast<TriangularShape*>(pair.a));
    else
        return;
}

void CollisionHandler::renderAabb(const Aabb& aabb, const glm::vec3& color)
{
    const glm::vec3& min = aabb.min;
    const glm::vec3& max = aabb.max;

    glm::vec3 a(min.x, max.y, min.z);
    glm::vec3 b(max.x, max.y, min.z);
    glm::vec3 c(min.x, min.y, min.z);
    glm::vec3 d(max.x, min.y, min.z);
    glm::vec3 e(min.x, max.y, max.z);
    glm::vec3 f(max.x, max.y, max.z);
    glm::vec3 g(min.x, min.y, max.z);
    glm::vec3 h(max.x, min.y, max.z);

    m_renderer->m_debugRenderer.addLine(a, b, color);
    m_renderer->m_debugRenderer.addLine(c, d, color);
    m_renderer->m_debugRenderer.addLine(a, c, color);
    m_renderer->m_debugRenderer.addLine(b, d, color);

    m_renderer->m_debugRenderer.addLine(e, f, color);
    m_renderer->m_debugRenderer.addLine(g, h, color);
    m_renderer->m_debugRenderer.addLine(e, g, color);
    m_renderer->m_debugRenderer.addLine(f, h, color);

    m_renderer->m_debugRenderer.addLine(e, a, color);
    m_renderer->m_debugRenderer.addLine(f, b, color);
    m_renderer->m_debugRenderer.addLine(g, c, color);
    m_renderer->m_debugRenderer.addLine(d, h, color);
}

void CollisionHandler::renderBounds()
{
    for (auto& ts : m_triangularShapes)
    {
        renderAabb(ts->getAabb(), glm::vec3(1.f,1.f,1.f));

        if (ts)
        {
            for (auto i = 0; i < ts->m_tris.size(); i++)
            {
                m_renderer->m_debugRenderer.addLine(ts->m_tris[i].verts[0], ts->m_tris[i].verts[1], vec3f(1.f));
                m_renderer->m_debugRenderer.addLine(ts->m_tris[i].verts[1], ts->m_tris[i].verts[2], vec3f(1.f));
                m_renderer->m_debugRenderer.addLine(ts->m_tris[i].verts[2], ts->m_tris[i].verts[0], vec3f(1.f));
            }
        }
    }

    for (auto& ss : m_sphereShapes)
    {
        m_renderer->m_debugRenderer.addAxis(ss->m_pos);
        renderAabb(ss->getAabb(), glm::vec3(1.f,1.f,1.f));
    }

    for (auto& es : m_ellipsoidShapes)
        renderAabb(es->getAabb(), glm::vec3(1.f,1.f,1.f));
}

void CollisionHandler::cachePairs()
{
    for (auto& tri : m_triangularShapes)
    for (auto& sph : m_sphereShapes)
    {
        if (aabbOverlapping(tri->getAabb(), sph->getAabb()))
        {
            m_pairs.emplace_back(tri, sph);
        }
    }

    for (auto& tri : m_triangularShapes)
    for (auto& eli : m_ellipsoidShapes)
    {
        if (aabbOverlapping(tri->getAabb(), eli->getAabb()))
        {
            m_pairs.emplace_back(tri, eli);
        }
    }
}

void CollisionHandler::resolveCollisions()
{
    cachePairs();

    for (auto& pair : m_pairs)
        doPair(pair);

    m_pairs.clear();

    if (m_renderer)
        renderBounds();
}
