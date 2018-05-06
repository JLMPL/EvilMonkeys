#pragma once
#include "Core/Types.hpp"
#include "Core/Math3d.hpp"
#include <string>
#include <vector>

struct Joint
{
    std::string name;

    mat4 nodeTransform;
    mat4 offsetMatrix;
    mat4 finalTransformation;
    mat4 attachment;

    std::vector<i8> children;
};

struct Skeleton
{
    std::vector<Joint> joints;

    i8 findJointIndex(const std::string& name);
    Joint*  findJoint(const std::string& name);
};
