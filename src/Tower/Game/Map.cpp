#include "Map.hpp"
#include "World.hpp"
#include "Resources/Resources.hpp"
#include "Render/Renderer3D.hpp"

void Map::init(World* world)
{
    m_world = world;

    m_planeRenderData.init(gResources.getMesh("demoScene0.obj").get());
    m_planeRenderData.material = gResources.getMaterial("Grass.mat").get();

    gRenderer3D.registerStaticMesh(&m_planeRenderData);

    m_walls.init(gResources.getMesh("Wallz.obj").get());
    m_walls.material = gResources.getMaterial("Wall.mat").get();

    gRenderer3D.registerStaticMesh(&m_walls);

    for (u32 i = 0; i < 15; i++)
    for (u32 j = 0; j < 15; j++)
    {
        u32 index = i * 15 + j;

        m_grass[index].init(vec3(0));
        m_grass[index].m_transform = glm::translate(vec3(i * 4, 0, j * 4) - vec3(28,0,28));
        m_grass[index].m_material = gResources.getMaterial("Grasse.mat").get();

        gRenderer3D.registerGrass(&m_grass[index]);
    }

    m_plantData.init(gResources.getMesh("hiplant.dae").get());
    m_plantData.material = gResources.getMaterial("Plant.mat").get();
    m_plantData.transform = math::translate(vec3(0,0,-5));

    gRenderer3D.registerFoliage(&m_plantData);
}

//PITS TITS AND NOUGHTY BITS
