#pragma once
#include "Core/Math3d.hpp"

class Mesh;
struct Material;

struct FoliageInstance
{
    const Mesh* mesh;
    const Material* material = nullptr;
    mat4 transform;

    void init(const Mesh* mesh);
};
