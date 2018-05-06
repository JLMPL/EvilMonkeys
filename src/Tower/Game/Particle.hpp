#pragma once
#include "Core/Math3d.hpp"

struct Particle
{
    vec3 pos;
    vec3 prevPos;
    vec3 dir;
    vec3 color;

    float velocity;
    float weight;

    seconds life;
    seconds lifetime;
};
