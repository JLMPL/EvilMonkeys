#pragma once
#include "ActorState.hpp"

class ActorDodge : public ActorState
{
    public:
        enum class Flags : u32
        {
            Left = 0,
            Right
        };

        void enter(Actor* actor, i32 flags = 0) override final;
        void sendMessage(const Message& event) override final;
        void update(seconds deltaTime) override final;

        bool isVulnerable() override final;

    private:
        Flags m_move;
};
