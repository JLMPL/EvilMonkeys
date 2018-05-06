#pragma once
#include "Render/3D/Mesh.hpp"
#include "Render/3D/Animation.hpp"
#include "Render/3D/AnimatedMesh.hpp"
#include "Render/2D/Font.hpp"
#include "Render/3D/Material.hpp"
#include "Render/Texture.hpp"
#include "Render/Shader.hpp"
#include <string>
#include <map>

class Resources
{
    public:
        void init();

        Texture::Ptr         getTexture(const std::string& name);
        Material::Ptr        getMaterial(const std::string& name);
        Mesh::Ptr            getMesh(const std::string& name);
        AnimatedMesh::Ptr    getAnimatedMesh(const std::string& name);
        Animation::Ptr       getAnimation(const std::string& name);
        Font::Ptr            getFont(const std::string& name, u32 size = 32);

    private:
        std::map<std::string, Shader::Ptr>          m_shaders;
        std::map<std::string, Texture::Ptr>         m_textures;
        std::map<std::string, Material::Ptr>        m_materials;
        std::map<std::string, Mesh::Ptr>            m_meshes;
        std::map<std::string, AnimatedMesh::Ptr>    m_animMeshes;
        std::map<std::string, Animation::Ptr>       m_animations;
        std::map<std::string, Font::Ptr>            m_fonts;
};

extern Resources gResources;
