#include "PlayerMovement.hpp"
#include "Messages/MessageBus.hpp"
#include "ActorDodge.hpp"
#include "../Actor.hpp"
#include "Core/Config.hpp"

void PlayerMovement::enter(Actor* actor, i32 flags)
{
    m_actor = actor;
}

void PlayerMovement::sendMessage(const Message& message)
{
    if (message.type == Message::Type::KeyPressed)
    {
        switch (message.keyboard.key)
        {
            case Config::Controls::Up:
                m_controls[0] = true;
                break;
            case Config::Controls::Down:
                m_controls[1] = true;
                break;
            case Config::Controls::Left:
                m_controls[2] = true;
                break;
            case Config::Controls::Right:
                m_controls[3] = true;
                break;
            case Config::Controls::Jump:
                m_dodge = true;
                break;
        }
    }
}

void PlayerMovement::update(seconds deltaTime)
{
    bool animNotFuckUp = false;
    if (m_controls[0])
    {
        m_actor->_pos += m_actor->_dir * m_actor->_speed * deltaTime.count();
        m_actor->setAnimation("Guy_run.dae");
        animNotFuckUp = true;

    }
    else if (m_controls[1])
    {
        m_actor->_pos -= m_actor->_dir * m_actor->_speed * deltaTime.count();
        m_actor->setAnimation("Guy_run.dae");
        animNotFuckUp = true;
    }

    vec3 side = math::cross(m_actor->_dir, math::vecY);
    side = math::normalize(side);

    if (m_controls[2])
    {
        m_actor->_pos -= side * m_actor->_speed * deltaTime.count();

        if (!animNotFuckUp)
            m_actor->setAnimation("Guy_strafeLeft.dae");

        if (m_dodge)
            setState(ActorState::Type::Dodge, u32(ActorDodge::Flags::Left));
    }
    else if (m_controls[3])
    {
        m_actor->_pos += side * m_actor->_speed * deltaTime.count();

        if (!animNotFuckUp)
            m_actor->setAnimation("Guy_strafeLeft.dae");

        if (m_dodge)
            setState(ActorState::Type::Dodge, u32(ActorDodge::Flags::Right));
    }

    if (!m_controls[0] &&
        !m_controls[1] &&
        !m_controls[2] &&
        !m_controls[3]
        )
    {
        setState(ActorState::Type::Idle);
    }

    m_dodge = false;
    memset(m_controls, 0, sizeof(bool) * 4);
}

bool PlayerMovement::isVulnerable()
{
    return true;
}
