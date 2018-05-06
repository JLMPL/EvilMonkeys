#pragma once
#include "Actor.hpp"
#include "PlayerController.hpp"
#include "HumanController.hpp"
#include "Decoration.hpp"
#include "Map.hpp"
#include "Weather.hpp"
#include "ParticleSystem.hpp"
#include "Render/3D/Camera.hpp"
#include "Interface/GameInterface.hpp"
#include <vector>

union SDL_Event;
struct Message;

class World
{
    public:
        World() = default;
        World(const World&) = delete;
        World& operator=(const World&) = delete;

        void init();
        void exit();
        void update(seconds deltaTime);

        void sendMessage(const Message& message);

        Actor*              closestEntityInRange(const vec3& pos, float range);
        Actor*              closestNotMe(Actor* ent, float range);
        std::vector<Actor*> getEntitiesInRange(const vec3& pos, float range);
        Actor*              getEntityById(u32 id);

    private:
        void setupSystems();
        void setupGameplay();

        void yesReally();

    private:
        ParticleSystem     m_particleSystem;

        Weather            m_weather;
        Map                m_map;

        std::vector<Actor::Ptr>      m_entities;
        std::vector<Decoration::Ptr> m_decorations;

        Camera              m_camera;
        PlayerController    m_player;
        HumanController     m_enemy;

        GameInterface       m_interface;

    friend class PlayerController;
    friend class ActorController;
};

//IM GONNA PUNCH YOU IN THE FUCKHOLE
