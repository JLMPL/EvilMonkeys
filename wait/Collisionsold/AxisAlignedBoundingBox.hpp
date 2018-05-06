#ifndef AXIS_ALIGNED_BOUNDING_BOX_HPP
#define AXIS_ALIGNED_BOUNDING_BOX_HPP
#include <glm/glm.hpp>
#include "Core/Types.hpp"

struct AxisAlignedBoundingBox
{
    glm::vec3 min;
    glm::vec3 max;

    void checkPoint(const glm::vec3& vec);
};

#endif