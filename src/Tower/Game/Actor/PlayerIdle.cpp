#include "PlayerIdle.hpp"
#include "ActorAttack.hpp"
#include "Messages/MessageBus.hpp"
#include "../Actor.hpp"
#include "Core/Config.hpp"

void PlayerIdle::enter(Actor* actor, i32 flags)
{
    m_actor = actor;

    if (!m_prep)
        m_actor->setAnimation("Guy_idle.dae");
    else
        m_actor->setAnimation("Guy_prep.dae", seconds(0.1));

    memset(m_controls, 0, sizeof(bool) * 4);
}

void PlayerIdle::sendMessage(const Message& message)
{
    if (message.type == Message::Type::KeyPressed)
    {
        if (message.keyboard.key == Config::Controls::Up)
        {
            m_controls[0] = true;
        }
        else if (message.keyboard.key == Config::Controls::Down)
        {
            m_controls[1] = true;
        }
        else if (message.keyboard.key == Config::Controls::Left)
        {
            m_controls[2] = true;
        }
        else if (message.keyboard.key == Config::Controls::Right)
        {
            m_controls[3] = true;
        }
        else if (message.keyboard.key == Config::Controls::Prep)
        {
            m_prep = true;

            if (m_prep)
            {
                m_actor->setAnimation("Guy_prep.dae", seconds(0.1));

                if (m_controls[0])
                    setState(ActorState::Type::Attack, 0);
                else if (m_controls[1])
                    setState(ActorState::Type::Attack, 1);
            }
        }
        else if (message.keyboard.key == Config::Controls::Jump)
        {
            if (m_controls[1])
                setState(ActorState::Type::Block);
        }
    }
}

void PlayerIdle::update(seconds deltaTime)
{
    if (m_prep)
    {
    }
    else if (m_controls[0] || m_controls[1] || m_controls[2] || m_controls[3])
    {
        setState(ActorState::Type::Movement);
    }
    else
    {
        m_actor->setAnimation("Guy_idle.dae");
    }

    m_prep = false;
}

bool PlayerIdle::isVulnerable()
{
    return true;
}
