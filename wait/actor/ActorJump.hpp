#ifndef ACTOR_JUMP_HPP
#define ACTOR_JUMP_HPP
#include "ActorState.hpp"

struct ActorJump : public ActorState
{
    seconds timer = seconds(0);

    void enter(Actor* actor) override final;
    void processEvents(const SDL_Event& event) override final;
    void update(seconds deltaTime) override final;
};

#endif
