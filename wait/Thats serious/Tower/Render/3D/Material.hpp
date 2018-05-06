#pragma once
#include <memory>
#include <string>

class Texture;

struct Material
{
    using Ptr = std::shared_ptr<Material>;

    const Texture* m_diffuse = nullptr;
    const Texture* m_normal = nullptr;

    void loadFromFile(const std::string& name);
};
