#pragma once
#include "ActorState.hpp"

class ActorAttack : public ActorState
{
    public:
        enum class Flags : u32
        {
            Forward = 0,
            Stab
        };

        void enter(Actor* actor, i32 flags = 0) override final;
        void sendMessage(const Message& event) override final;
        void update(seconds deltaTime) override final;

        bool isVulnerable() override final;

    private:
        Flags m_flags;
        bool m_attackNow = false;
};
