#pragma once
#include "Aabb.hpp"
#include "Core/Math3d.hpp"

namespace Shape
{
    struct BaseShape
    {
        enum class Type
        {
            Sphere,
            Ellipsoid,
            Polyhedron
        };

        Type    type;
        Aabb    aabb;

        mat4    transform;
        vec3    pos;

        void move(const vec3& offset);
        Aabb getAabb();
    };
}
