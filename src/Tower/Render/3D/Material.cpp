#include "Material.hpp"
#include "Resources/Resources.hpp"
#include "Core/Log.hpp"
#include <fstream>

void Material::loadFromFile(const std::string& name)
{
    std::ifstream file(name.c_str());
    std::string line;

    if (!file.good())
    {
        Log::log(Log::Type::Error, "Could not load Material %s\n", name.c_str());
        exit(EXIT_FAILURE);
    }

    std::getline(file, line);
    m_diffuse = gResources.getTexture(line).get();

    std::getline(file, line);
    m_normal = gResources.getTexture(line).get();

    file.close();
}
