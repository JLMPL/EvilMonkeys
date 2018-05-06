#pragma once
#include "Core/Math3d.hpp"
#include <memory>

struct Light
{
    using Ptr = std::unique_ptr<Light>;

    vec3 pos;
    vec3 color;
    float intensity = 1.f;

    mat4 transform;
};

//IT'S THE GREAF-OFF
