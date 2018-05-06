#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP
#include "State.hpp"
#include "Game/World.hpp"

class GameState : public State
{
    public:
        GameState(StateStack* stack, Renderer* renderer);

        bool processEvents(const SDL_Event& event) override final;
        bool update(float deltaTime) override final;
        void render() override final;

    private:
        World m_world;
};

#endif