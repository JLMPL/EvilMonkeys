#include "Shape.hpp"

namespace Shape
{
    void BaseShape::move(const vec3& offset)
    {
        pos += offset;
    }

    Aabb BaseShape::getAabb()
    {
        return {aabb.min + pos, aabb.max + pos};
    }
}
