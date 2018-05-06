#include "World.hpp"
#include "Actor.hpp"
#include "Core/Convert.hpp"
#include "Render/Renderer3D.hpp"
#include "Resources/Resources.hpp"
#include "Messages/MessageBus.hpp"
#include "Core/Config.hpp"
#include <SDL2/SDL.h>
#include <algorithm>

void World::setupSystems()
{
    m_particleSystem.init();
}

void World::setupGameplay()
{
    m_entities.emplace_back(new Actor());
    m_entities.back()->init(this);
    m_entities.back()->_pos = vec3(0,0,10);

    m_entities.emplace_back(new Actor());
    m_entities.back()->init(this);
    m_entities.back()->_pos = vec3(0);
}

void World::init()
{
    setupSystems();

    m_map.init(this);
    m_weather.init();

    setupGameplay();

    m_camera.init();
    gRenderer3D.setActiveCamera(&m_camera);

    m_player.init(m_entities[0].get(), this, &m_camera);
    m_enemy.init(m_entities[1].get(), this);

    m_interface.init();
}

void World::exit()
{
    m_interface.exit();
}

void World::sendMessage(const Message& message)
{
    m_camera.sendMessage(message);
    m_player.sendMessage(message);
    m_enemy.sendMessage(message);
    m_interface.sendMessage(message);
    m_particleSystem.sendMessage(message);

    for (auto& ent : m_entities)
    {
        ent->sendMessage(message);
    }
}

void World::yesReally()
{
    vec3 diste = m_entities[0]->_pos - m_entities[1]->_pos;
    float dist = math::length(diste);

    if (dist < 4)
    {
        float diff = 4 - dist;

        vec3 mov = math::normalize(diste);

        m_entities[0]->_pos += (mov * diff * 0.5f);
        m_entities[1]->_pos += (-mov * diff * 0.5f);
    }
}

void World::update(seconds deltaTime)
{
    m_weather.update(deltaTime);

    // for (u32 i = 0; i < 10; i++)
    // for (u32 j = 0; j < 10; j++)

    m_player.update(deltaTime);
    m_enemy.update(deltaTime);

    for (auto& i : m_entities)
        i->preUpdate(deltaTime);

    yesReally();

    for (auto& i : m_entities)
        i->postUpdate(deltaTime);

    m_particleSystem.update(deltaTime);

    m_camera.update();
}

Actor* World::closestEntityInRange(const vec3& pos, float range)
{
    Actor* found = nullptr;
    float closest = range;

    for (auto& i : m_entities)
    {
        float theDist = math::distance(pos, i->_pos);
        if (theDist < closest && theDist > 0.1f)
        {
            closest = theDist;
            found = i.get();
        }
    }

    return found;
}

Actor* World::closestNotMe(Actor* ent, float range)
{
    auto ents = getEntitiesInRange(ent->_pos, range);

    for (auto& i : ents)
        if (i != ent)
            return i;

    return nullptr;
}

std::vector<Actor*> World::getEntitiesInRange(const vec3& pos, float range)
{
    std::vector<Actor*> ents;

    for (auto& entity : m_entities)
    {
        float dist = math::distance(pos, entity->_pos);
        if (dist < range && dist > 0.1f)
        {
            ents.push_back(entity.get());
        }
    }

    std::sort(ents.begin(), ents.end(),
    [&](const Actor* a, const Actor* b)
    {
        return math::distance2(a->_pos, pos) < math::distance2(b->_pos, pos);
    });

    return ents;
}

Actor* World::getEntityById(u32 id)
{
    for (auto i = 0; i < m_entities.size(); i++)
        if (m_entities[i]->_id == id)
            return m_entities[i].get();

    return nullptr;
}

//IMMA SEXUAL STEVE JOBS, AND THAT BITCH IS WORTH 10 POINTS
