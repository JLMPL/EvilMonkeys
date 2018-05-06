#ifndef PLAYER_MOVEMENT_HPP
#define PLAYER_MOVEMENT_HPP
#include "ActorState.hpp"

struct PlayerMovement : public ActorState
{
    void enter(Actor* actor) override final;
    void processEvents(const SDL_Event& event) override final;
    void update(seconds deltaTime) override final;
};

#endif
