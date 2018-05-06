#pragma once
#include "../Shape.hpp"

namespace Shape
{
    struct Ellipsoid : public BaseShape
    {
        vec3 radii;

        void init(const vec3& radii);
    };
}
