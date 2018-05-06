#pragma once
#include "Render/3D/Mesh.hpp"
#include "Render/3D/Light.hpp"
#include "Render/3D/MeshInstance.hpp"
#include "Script/Lua.hpp"
#include <memory>

class Decoration
{
    public:
        using Ptr = std::unique_ptr<Decoration>;

        Decoration() = default;
        ~Decoration() = default;

        void init(const mat4& tr);
        void setTransform(const mat4& parent);

    private:
        // void initMesh(const sol::table& lua);
        // void initLight(const sol::table& lua);

    private:
        mat4         m_globalTransform;
        mat4         m_localTransform;

        MeshInstance    m_meshSceneData;
        mat4         m_meshLocal;

        Light        m_light;
        mat4         m_lightLocal;
        mat4         m_lightGlobal;
};
