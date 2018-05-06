#include "SplashState.hpp"
#include "StateStack.hpp"
#include "Render/Renderer.hpp"
#include <SDL/SDL.h>

SplashState::SplashState(StateStack* stack, Renderer* renderer)
{
    m_stateStack = stack;
    m_renderer = renderer;

    m_texture.loadFromFile("Fonts/shit.png");
    m_font.loadFromFile("Fonts/shit.fnt");
    m_text.init(&m_font);
    m_text.setString("Yeahh! Text is back on the screen!");

    m_renderer->registerTextData(&m_text, &m_texture, nullptr);
}

SplashState::~SplashState()
{
    m_renderer->clearAllData();
}

bool SplashState::processEvents(const SDL_Event& event)
{
    printf("fucking processing\n");
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
    {
        // m_stateStack->clear();
        m_text.setString("I pressed motherfuckin key\n");
        printf("fucking keydown\n");
    }

    return true;
}

bool SplashState::update(float deltaTime)
{
    return true;
}

void SplashState::render()
{

}
