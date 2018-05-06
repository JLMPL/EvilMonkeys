#include "Decoration.hpp"
#include "Resources/Resources.hpp"
#include "Render/Renderer3D.hpp"
#include "Script/Lua.hpp"

// void Decoration::initMesh(const sol::table& lua)
// {
//     std::string mesh = lua["data"];
//     std::string mat = lua["material"];

//     vec3 pos = vec3(
//         lua["pos"][1],
//         lua["pos"][2],
//         lua["pos"][3]
//     );

//     m_meshLocal = m_localTransform * math::translate(pos);
//     m_meshSceneData.initStatic(
//         Resources::getMesh(mesh).get(),
//         Resources::getMaterial(mat).get(),
//         mat4()
//     );
//     g_renderer.addSceneData(&m_meshSceneData);
// }

// void Decoration::initLight(const sol::table& lua)
// {
//     m_light.color = vec3(
//         lua["color"]["r"],
//         lua["color"]["g"],
//         lua["color"]["b"]
//     );
//     m_light.intensity = lua["intensity"];
//     vec3 pos = vec3(
//         lua["pos"]["x"],
//         lua["pos"]["y"],
//         lua["pos"]["z"]
//     );

//     m_lightLocal = math::translate(pos);
//     m_light.pos = (m_localTransform * m_lightLocal)[3];

//     g_renderer.addLight(&m_light);
// }

void Decoration::init(const mat4& tr)
{
    // sol::state lua;
    // lua.open_libraries(sol::lib::base, sol::lib::package);
    // lua.script_file("Entities/Decorations/sword.lua");

    // m_localTransform = mat4(1.f);

    // bool isMesh = lua["Deco"]["Mesh"]["is"];
    // if (isMesh)
    //     initMesh(lua["Deco"]["Mesh"]);

    // bool isLight = lua["Deco"]["Light"]["is"];
    // if (isLight)
    //     initLight(lua["Deco"]["Light"]);

    m_meshLocal = m_localTransform * math::translate(vec3(0));
    m_meshSceneData.init(gResources.getMesh("sord.obj").get());
    gRenderer3D.registerStaticMesh(&m_meshSceneData);

    setTransform(tr);
}

void Decoration::setTransform(const mat4& parent)
{
    m_globalTransform = parent * m_localTransform;

    m_meshSceneData.transform = parent * m_meshLocal;
    m_lightGlobal = parent * m_lightLocal;

    m_light.pos = vec3(m_lightGlobal[3]);
}
