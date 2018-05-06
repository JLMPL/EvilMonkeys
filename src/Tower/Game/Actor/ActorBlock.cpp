#include "ActorBlock.hpp"
#include "Messages/MessageBus.hpp"
#include "../Actor.hpp"
#include "Core/Random.hpp"

void ActorBlock::enter(Actor* actor, i32 flags)
{
    m_actor = actor;

    m_actor->setAnimation("Guy_block.dae", seconds(0.002), false);
}

void ActorBlock::sendMessage(const Message& event)
{
}

void ActorBlock::update(seconds deltaTime)
{
    if (m_actor->_animMesh.isFinished())
    {
        setState(ActorState::Type::Idle);
    }
}

bool ActorBlock::isVulnerable()
{
    return false;
}
