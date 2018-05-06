#pragma once
#include "../Shape.hpp"
#include <vector>

namespace Shape
{
    struct Triangle
    {
        vec3 verts[3];
        vec3 normal;
    };

    struct Polyhedron : public BaseShape
    {
        std::vector<Triangle> triangles;

        void init();
        void addTriangle(const Triangle& tri);
    };
}
