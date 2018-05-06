#include "Resources.hpp"
#include "Core/Log.hpp"

Resources gResources;

void Resources::init()
{
    //THERE'S NOTHING YOU HAVE BEEN CHEATED
    //HAHAHAHAHAHAHAHA
}

Texture::Ptr Resources::getTexture(const std::string& name)
{
    auto found = m_textures.find(name);

    if (found == m_textures.end())
    {
        auto texture = std::make_shared<Texture>();
        texture->loadFromFile(std::string("Textures/" + name).c_str());
        m_textures[name] = texture;
    }

    return m_textures[name];
}

Material::Ptr Resources::getMaterial(const std::string& name)
{
    auto found = m_materials.find(name);

    if (found == m_materials.end())
    {
        auto material = std::make_shared<Material>();
        material->loadFromFile("Materials/" + name);
        m_materials[name] = material;
    }

    return m_materials[name];
}

Mesh::Ptr Resources::getMesh(const std::string& name)
{
    auto found = m_meshes.find(name);

    if (found == m_meshes.end())
    {
        auto mesh = std::make_shared<Mesh>();
        mesh->loadFromFile("Meshes/" + name);
        m_meshes[name] = mesh;
    }

    return m_meshes[name];
}

AnimatedMesh::Ptr Resources::getAnimatedMesh(const std::string& name)
{
    auto found = m_animMeshes.find(name);

    if (found == m_animMeshes.end())
    {
        auto animMesh = std::make_shared<AnimatedMesh>();
        animMesh->loadFromFile("Meshes/" + name);
        m_animMeshes[name] = animMesh;
    }

    return m_animMeshes[name];
}

Animation::Ptr Resources::getAnimation(const std::string& name)
{
    auto found = m_animations.find(name);

    if (found == m_animations.end())
    {
        auto anim = std::make_shared<Animation>();
        anim->loadFromFile("Animations/" + name);
        m_animations[name] = anim;
    }

    return m_animations[name];
}

Font::Ptr Resources::getFont(const std::string& name, u32 size)
{
    auto found = m_fonts.find(name);

    if (found == m_fonts.end())
    {
        auto font = std::make_shared<Font>();
        font->loadFromFile("Fonts/" + name);
        m_fonts[name] = font;
    }

    return m_fonts[name];
}
