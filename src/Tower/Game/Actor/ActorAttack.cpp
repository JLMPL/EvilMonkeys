#include "ActorAttack.hpp"
#include "Messages/MessageBus.hpp"
#include "../Actor.hpp"

constexpr f32 gWeaponRange = 10.f;

void ActorAttack::enter(Actor* actor, i32 flags)
{
    m_actor = actor;

    m_flags = Flags(flags);

    if (m_flags == Flags::Forward)
        m_actor->setAnimation("Guy_attack.dae", seconds(0.2), false, &m_attackNow, seconds(0.35));
    else
        m_actor->setAnimation("Guy_stab.dae", seconds(0.001), false, &m_attackNow, seconds(0.25));

    m_attackNow = false;

    Message message(Message::Type::ActorAboutToAttack);
    message.recieverEntity = m_actor->_focus->_id;
    MessageBus::queueMessage(message);

    Message messagee(Message::Type::SoundPlay);
    messagee.ivalue = 1;
    MessageBus::queueMessage(messagee);
}

void ActorAttack::sendMessage(const Message& event)
{
}

void ActorAttack::update(seconds deltaTime)
{
    if (m_attackNow && math::distance(m_actor->_focus->_pos, m_actor->_pos) <= gWeaponRange)
    {
        Message message(Message::Type::ActorDamage);

        if (m_flags == Flags::Forward)
            message.ivalue = 15;
        else
            message.ivalue = 10;

        message.recieverEntity = m_actor->_focus->_id;

        MessageBus::queueMessage(message);

        m_attackNow = false;
    }

    if (m_actor->_animMesh.isFinished())
    {
        setState(ActorState::Type::Idle);
    }
}

bool ActorAttack::isVulnerable()
{
    return true;
}
