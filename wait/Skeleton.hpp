#ifndef SKELETON_HPP
#define SKELETON_HPP
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Weight
{
    uint32_t vertexID;
    float weight;
};

struct Bone
{
    std::string name;

    glm::mat4 nodeTransform;
    glm::mat4 offsetMatrix;
    glm::mat4 finalTransformation;

    std::vector<Weight> Weights;
    std::vector<int8_t> Children;
};

struct Skeleton
{
    std::vector<Bone> bones;

    int8_t findBoneIndex(const std::string& name);
    Bone*  findBone(const std::string& name);
};

#endif