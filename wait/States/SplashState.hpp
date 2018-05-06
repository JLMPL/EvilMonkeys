#ifndef SPLASH_STATE_HPP
#define SPLASH_STATE_HPP
#include "State.hpp"
#include "Render/Texture.hpp"
#include "Render/Font.hpp"
#include "Render/Text.hpp"

class Renderer;

class SplashState : public State
{
    public:
        SplashState(StateStack* stack, Renderer* renderer);
        ~SplashState();

        bool processEvents(const SDL_Event& event) override final;
        bool update(float deltaTime) override final;
        void render() override final;

    private:
        Renderer* m_renderer = nullptr;

        Texture m_texture;
        Font m_font;
        Text m_text;
};

#endif