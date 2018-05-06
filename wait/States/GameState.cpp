#include "GameState.hpp"
#include "StateStack.hpp"
#include <SDL2/SDL.h>

GameState::GameState(StateStack* stack, Renderer* renderer)
{
    m_stateStack = stack;
    m_world.init(renderer);
}

bool GameState::processEvents(const SDL_Event& event)
{
    m_world.processEvents(event);

    if (event.type == SDL_KEYDOWN)
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            m_stateStack->clear();
            m_stateStack->push(State::Type::Splash);
            printf("FUCK0\n");
        }

    return true;
}

bool GameState::update(float deltaTime)
{
    m_world.update(deltaTime);
    return true;
}

void GameState::render()
{
}
