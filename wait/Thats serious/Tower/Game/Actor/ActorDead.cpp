#include "ActorDead.hpp"
#include "Messages/MessageBus.hpp"
#include "GameState/GameState.hpp"
#include "../Actor.hpp"

void ActorDead::enter(Actor* actor, i32 flags)
{
    m_actor = actor;

    m_actor->setAnimation("Guy_death.dae", seconds(0.2), false);

    {
        Message message(Message::Type::ParticlesSpawn);
        message.particles.num = 512;

        message.particles.pos.x = m_actor->_pos.x;
        message.particles.pos.y = 6;
        message.particles.pos.z = m_actor->_pos.z;

        message.particles.dir.x = 0;
        message.particles.dir.y = 1;
        message.particles.dir.z = 0;

        message.particles.color.x = 1;
        message.particles.color.y = 0;
        message.particles.color.z = 0;

        message.particles.velocity = 25;
        message.particles.weight = 2.f;

        message.particles.life = 5;

        MessageBus::queueMessage(message);
    }

    Message message(Message::Type::SoundPlay);
    message.ivalue = 0;
    MessageBus::queueMessage(message);

    m_timer = seconds(0);
}

void ActorDead::sendMessage(const Message& event)
{
}

void ActorDead::update(seconds deltaTime)
{
    m_timer += deltaTime;

    if (m_timer > seconds(5) && !m_sent)
    {
        Message message(Message::Type::GameStateSet);
        message.state = i32(GameState::Type::Over);
        MessageBus::queueMessage(message);
        m_sent = true;
    }
}

bool ActorDead::isVulnerable()
{
    return false;
}
