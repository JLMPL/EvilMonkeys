#include "HumanIdle.hpp"
#include "../Actor.hpp"
#include "Core/Random.hpp"

void HumanIdle::enter(Actor* actor, i32 flags)
{
    m_actor = actor;

    m_actor->setAnimation("Guy_idle.dae");
}

void HumanIdle::sendMessage(const Message& message)
{
}

void HumanIdle::update(seconds deltaTime)
{
    f32 dist = math::distance(m_actor->_pos, m_actor->_focus->_pos);

    if (dist > 10.f && dist < 50.f)
    {
        setState(ActorState::Type::Movement);
    }
    else
    {
        // u32 doShit = Random::inRangei(0,1);

        // switch (doShit)
        // {
        //     case 0:
        //         setState(ActorState::Type::Attack);
        //         break;
        //     case 1:
        //         setState(ActorState::Type::Block);
        //         break;
        // }
    }
}

bool HumanIdle::isVulnerable()
{
    return true;
}
