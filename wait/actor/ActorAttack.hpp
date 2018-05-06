#ifndef ACTOR_ATTACK_HPP
#define ACTOR_ATTACK_HPP
#include "ActorState.hpp"

struct ActorAttack : public ActorState
{
    bool m_attackNow = false;
    bool m_move = true;

    void enter(Actor* actor) override final;
    void processEvents(const SDL_Event& event) override final;
    void update(seconds deltaTime) override final;
};

#endif
