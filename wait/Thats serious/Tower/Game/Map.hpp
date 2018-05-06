#pragma once
#include "Render/3D/Mesh.hpp"
#include "Render/3D/MeshInstance.hpp"
#include "Render/3D/FoliageInstance.hpp"
#include "Render/3D/GrassInstance.hpp"

class World;

class Map
{
    public:
        Map() = default;
        ~Map() = default;

        void init(World* world);

    private:
        World* m_world = nullptr;

        MeshInstance m_planeRenderData;
        MeshInstance m_walls;

        GrassInstance m_grass[15*15];
        FoliageInstance m_plantData;
};

//SMILE IS LIKE UNDERWEAR IT LIFTS YOUR CHEEKS
