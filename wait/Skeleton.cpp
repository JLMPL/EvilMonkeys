#include "Skeleton.hpp"

int8_t Skeleton::findBoneIndex(const std::string& name)
{
    for (size_t i = 0; i < bones.size(); i++)
    {
        if (bones[i].name == name)
            return i;
    }
    return -1;
}

Bone* Skeleton::findBone(const std::string& name)
{
    return &bones[findBoneIndex(name)];
}