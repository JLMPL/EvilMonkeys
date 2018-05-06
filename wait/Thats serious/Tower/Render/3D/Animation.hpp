#pragma once
#include "Core/Math3d.hpp"
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

    std::vector<NodeAnimationKey<vec3>> positionKeys;
    std::vector<NodeAnimationKey<quat>> rotationKeys;
};

struct Animation
{
    using Ptr = std::shared_ptr<Animation>;

    std::vector<NodeAnimation> nodeAnimations;

    float timeIn = 0;
    bool looping = true;
    float triggerTime = 0;

    seconds duration = seconds(0);

    void loadFromFile(const Path& path);
    void loadCollada(const Path& path);
    const NodeAnimation* findNodeAnim(const std::string& nodeName) const;
};
