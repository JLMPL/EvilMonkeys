#pragma once
#include "Actor/ActorState.hpp"
#include "Core/Types.hpp"

struct Message;
struct Actor;
class World;

class HumanController
{
    public:
        enum class CombatAction
        {
            Attack,
            Stab,
            StrafeLeft,
            StrafeRight,
            Block
        };

        HumanController() = default;

        void init(Actor* actor, World* world);
        void update(seconds deltaTime);

        void sendMessage(const Message& message);

    private:
        void initStates();

        void pickDefensiveChain();
        void pickOffensiveChain();

        void executeChainStep(CombatAction action);

        void setState(ActorState::Type state, u32 flags);

    private:
        World* m_world = nullptr;
        Actor* m_actor = nullptr;
        ActorState* m_state = nullptr;
        ActorState::Ptr m_states[ActorState::Type::Count];

        i32 m_currentChain = -1;
        i32 m_chainStep = 0;

        seconds m_timer {0};
};
