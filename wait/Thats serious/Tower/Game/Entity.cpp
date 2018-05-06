#include "Entity.hpp"

static u32 g_entityId = 0;

Entity::Entity()
{
    m_id = g_entityId++;
}

void Entity::move(const vec3& offset)
{
    m_pos += offset;
}

void Entity::setId(u32 id)
{
    m_id = id;
}

u32 Entity::getId() const
{
    return m_id;
}

void Entity::setPosition(const vec3& pos)
{
    m_pos = pos;
}

const vec3& Entity::getPosition() const
{
    return m_pos;
}

void Entity::destroy()
{
    m_destroyed = true;
}

bool Entity::isDestroyed() const
{
    return m_destroyed;
}
