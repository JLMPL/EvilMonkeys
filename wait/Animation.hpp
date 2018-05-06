#ifndef ANIMATION_HPP
#define ANIMATION_HPP
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <memory>
#include <vector>

template<typename T>
struct NodeAnimationKey
{
    T value;
    float time;
};

struct NodeAnimation
{
    std::string name;

    std::vector<NodeAnimationKey<glm::vec3>> positionKeys;
    std::vector<NodeAnimationKey<glm::quat>> rotationKeys;
};

struct Animation
{
    using Ptr = std::shared_ptr<Animation>;

    std::vector<NodeAnimation> nodeAnimations;

    float ticksPerSecond = 0;
    float duration = 0;

    bool loadFromFile(const std::string& path);
    const NodeAnimation* findNodeAnim(const std::string& nodeName) const;
};

#endif