#include "HumanMovement.hpp"
#include "../Actor.hpp"

void HumanMovement::enter(Actor* actor, i32 flags)
{
    m_actor = actor;

    m_actor->setAnimation("Guy_run.dae");
}

void HumanMovement::sendMessage(const Message& message)
{
}

void HumanMovement::update(seconds deltaTime)
{
    f32 dist = math::distance(m_actor->_pos, m_actor->_focus->_pos);

    if (dist < 10.f || dist > 50.f)
        setState(ActorState::Type::Idle);

    m_actor->_pos += m_actor->_dir * m_actor->_speed * deltaTime.count();
}

bool HumanMovement::isVulnerable()
{
    return true;
}
