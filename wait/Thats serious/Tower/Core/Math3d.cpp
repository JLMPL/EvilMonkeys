#include "Math3d.hpp"

namespace glm
{
    mat4 rotate(float angle, const vec3& axis)
    {
        return glm::rotate(mat4(1), angle, axis);
    }

    float sqrDistance(const vec3& a, const vec3& b)
    {
        vec3 tmp = a - b;
        return tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z;
    }
}
