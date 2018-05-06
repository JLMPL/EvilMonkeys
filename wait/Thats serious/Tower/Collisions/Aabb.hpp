#pragma once
#include "Core/Math3d.hpp"

struct Aabb
{
    vec3 min;
    vec3 max;

    void checkPoint(const vec3& vec);
};
