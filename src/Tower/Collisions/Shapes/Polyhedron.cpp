#include "Polyhedron.hpp"

namespace Shape
{
    void Polyhedron::init()
    {
        type = BaseShape::Type::Polyhedron;

        for (u32 i = 0; i < triangles.size(); i++)
        {
            aabb.checkPoint(triangles[i].verts[0]);
            aabb.checkPoint(triangles[i].verts[1]);
            aabb.checkPoint(triangles[i].verts[2]);
        }
    }

    void Polyhedron::addTriangle(const Triangle& tri)
    {
        triangles.push_back(tri);
    }
}
