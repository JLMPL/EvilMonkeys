#pragma once
#include "Math3d.hpp"

/* no bullet
static btVector3 glmToBt(const vec3& v)
{
    return btVector3(v.x, v.y, v.z);
}

static btQuaternion glmToBt(const quat& q)
{
    return btQuaternion(q.x, q.y, q.z, q.w);
}

static btTransform glmToBt(const mat4& m)
{
    btTransform tr;
    tr.setOrigin(glmToBt(vec3(m[3])));

    return tr;
}

static vec3 btToGlm(const btVector3& v)
{
    return vec3(v.x(), v.y(), v.z());
}

static quat btToGlm(const btQuaternion& q)
{
    return quat(q.x(), q.y(), q.z(), q.w());
}

static mat4 btToGlm(const btTransform& m)
{
    float array[16];
    m.getOpenGLMatrix(array);

    return math::make_mat4(array);
}
*/
