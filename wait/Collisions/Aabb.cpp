#include "Aabb.hpp"

void Aabb::checkPoint(const glm::vec3& vec)
{
    if (vec.x < min.x)
        min.x = vec.x;

    if (vec.y < min.y)
        min.y = vec.y;

    if (vec.z < min.z)
        min.z = vec.z;

    if (vec.x > max.x)
        max.x = vec.x;

    if (vec.y > max.y)
        max.y = vec.y;

    if (vec.z > max.z)
        max.z = vec.z;
}
