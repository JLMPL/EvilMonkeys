#include "ActorDodge.hpp"
#include "ActorAttack.hpp"
#include "Messages/MessageBus.hpp"
#include "../Actor.hpp"

void ActorDodge::enter(Actor* actor, i32 flags)
{
    m_actor = actor;

    m_move = Flags(flags);

    if (m_move == Flags::Left)
        m_actor->setAnimation("Guy_dodgeLeft.dae", seconds(0.05), false);
    else
        m_actor->setAnimation("Guy_dodgeRight.dae", seconds(0.05), false);

    Message messagee(Message::Type::SoundPlay);
    messagee.ivalue = 2;
    MessageBus::queueMessage(messagee);
}

void ActorDodge::sendMessage(const Message& message)
{

}

void ActorDodge::update(seconds deltaTime)
{
    vec3 dire = math::normalize(math::cross(m_actor->_dir, math::vecY));

    if (m_move == Flags::Left)
    {
        m_actor->_pos -= dire * 8.f * deltaTime.count();
    }
    else
    {
        m_actor->_pos += dire * 8.f * deltaTime.count();
    }

    if (m_actor->_animMesh.isFinished())
        setState(ActorState::Type::Idle);
}

bool ActorDodge::isVulnerable()
{
    return false;
}
