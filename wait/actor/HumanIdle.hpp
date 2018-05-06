#ifndef HUMAN_IDLE_HPP
#define HUMAN_IDLE_HPP
#include "ActorState.hpp"

struct HumanIdle : public ActorState
{
    seconds m_timer;

    void enter(Actor* actor) override final;
    void processEvents(const SDL_Event& event) override final;
    void update(seconds deltaTime) override final;
};

#endif
