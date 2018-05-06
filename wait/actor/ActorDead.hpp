#ifndef ACTOR_DEAD_HPP
#define ACTOR_DEAD_HPP
#include "ActorState.hpp"

#define BEGIN_ACTOR_STATE(x)    struct x : public ActorState {

#define END_ACTOR_STATE()    void enter(Actor* actor) override final;\
void processEvents(const SDL_Event& event) override final;\
void update(seconds deltaTime) override final;\
};

BEGIN_ACTOR_STATE(ActorDead)
END_ACTOR_STATE()

// struct ActorDead : public ActorState
// {
//     void enter(Actor* actor) override final;
//     void processEvents(const SDL_Event& event) override final;
//     void update(seconds deltaTime) override final;
// };

#endif
