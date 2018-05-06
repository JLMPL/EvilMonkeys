#include "ActorPain.hpp"
#include "Messages/MessageBus.hpp"
#include "../Actor.hpp"

void ActorPain::enter(Actor* actor, i32 flags)
{
    m_actor = actor;

    m_actor->setAnimation("Guy_pain.dae", seconds(0.2), false);

    {
        Message message(Message::Type::ParticlesSpawn);
        message.particles.num = 32;

        message.particles.pos.x = m_actor->_pos.x;
        message.particles.pos.y = 6;
        message.particles.pos.z = m_actor->_pos.z;

        message.particles.dir.x = 1;
        message.particles.dir.y = 1;
        message.particles.dir.z = 1;

        message.particles.color.x = 1;
        message.particles.color.y = 0;
        message.particles.color.z = 0;

        message.particles.velocity = 10;
        message.particles.weight = 4.f;

        message.particles.life = 5;

        MessageBus::queueMessage(message);
    }

    Message message(Message::Type::SoundPlay);
    message.ivalue = 0;
    MessageBus::queueMessage(message);
}

void ActorPain::sendMessage(const Message& event)
{
}

void ActorPain::update(seconds deltaTime)
{
    if (m_actor->_animMesh.isFinished())
    {
        setState(ActorState::Type::Idle);
    }
}

bool ActorPain::isVulnerable()
{
    return true;
}
