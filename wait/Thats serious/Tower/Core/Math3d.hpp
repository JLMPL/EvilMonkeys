#pragma once
#include "Types.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using vec2  = glm::tvec2<float, (glm::precision)0u>;
using vec2i = glm::tvec2<i32, (glm::precision)0u>;
using vec2u = glm::tvec2<u32, (glm::precision)0u>;
using vec3  = glm::tvec3<float, (glm::precision)0u>;
using vec4  = glm::tvec4<float, (glm::precision)0u>;

using vec4u = glm::tvec4<u32, (glm::precision)0u>;

using mat3  = glm::tmat3x3<float, (glm::precision)0u>;
using mat4  = glm::tmat4x4<float, (glm::precision)0u>;

using quat  = glm::tquat<float, (glm::precision)0u>;

namespace glm
{

#ifdef _WIN32
const vec3 vecX = {1,0,0};
const vec3 vecY = {0,1,0};
const vec3 vecZ = {0,0,1};
const vec3 vec0 = {0,0,0};
#else
constexpr vec3 vecX = {1,0,0};
constexpr vec3 vecY = {0,1,0};
constexpr vec3 vecZ = {0,0,1};
constexpr vec3 vec0 = {0,0,0};
#endif

extern mat4 rotate(float angle, const vec3& axis);
extern float sqrDistance(const vec3& a, const vec3& b);

}

namespace math = glm;
