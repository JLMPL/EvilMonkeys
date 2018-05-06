#ifndef COLLISION_HPP
#define COLLISION_HPP
#include "Shapes.hpp"
#include <glm/glm.hpp>

constexpr float32 Epsilon = 0.00001f;

//Narrow phase
bool rayTriangle(const CollisionTriangle& triangle, const glm::vec3& from, const glm::vec3& to, float& out);
glm::vec3 pointEdge(const glm::vec3& a, const glm::vec3& b, const glm::vec3& point);

//Broadphase
bool aabbOverlapping(const AxisAlignedBoundingBox& a, const AxisAlignedBoundingBox& b);

/*
Spatial Partitioning
*/

#endif
