#pragma once
#include "../Shape.hpp"

namespace Shape
{
    struct Sphere : public BaseShape
    {
        float radius = 1.f;

        void init(f32 radius);
    };
}
