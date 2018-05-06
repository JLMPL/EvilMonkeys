#ifndef ACTOR_CLIMB_HPP
#define ACTOR_CLIMB_HPP
#include "ActorState.hpp"
#include "Core/Math3d.hpp"

struct ActorClimb : public ActorState
{
    vec3f m_ledgePos;
    vec3f m_ledgeNormal;

    void enter(Actor* actor) override final;
    void processEvents(const SDL_Event& event) override final;
    void update(seconds deltaTime) override final;
};

#endif
