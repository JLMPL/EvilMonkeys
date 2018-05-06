#pragma once
#include "Shape.hpp"
#include "Shapes/Polyhedron.hpp"

constexpr f32 Epsilon = 0.00001f;

bool rayTriangle(const Shape::Triangle& triangle, const vec3& from, const vec3& to, float& out);
vec3 pointEdge(const vec3& a, const vec3& b, const vec3& point);

bool aabbOverlapping(const Aabb& a, const Aabb& b);
