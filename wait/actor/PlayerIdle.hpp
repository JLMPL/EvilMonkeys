#ifndef PLAYER_IDLE_HPP
#define PLAYER_IDLE_HPP
#include "ActorState.hpp"

struct PlayerIdle : public ActorState
{
    bool m_focused = false;

    void enter(Actor* actor) override final;

    void processEvents(const SDL_Event& event) override final;

    void updateFree(seconds deltaTime);
    void updateFocused(seconds deltaTime);
    void update(seconds deltaTime) override final;
};

#endif
