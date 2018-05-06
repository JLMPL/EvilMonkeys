#include "Skeleton.hpp"

i8 Skeleton::findJointIndex(const std::string& name)
{
    for (size_t i = 0; i < joints.size(); i++)
    {
        if (joints[i].name == name)
            return i;
    }
    return -1;
}

Joint* Skeleton::findJoint(const std::string& name)
{
    return &joints[findJointIndex(name)];
}
