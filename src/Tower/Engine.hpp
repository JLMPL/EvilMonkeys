#pragma once
#include "GameState/GameState.hpp"
#include "Input/Input.hpp"
#include "Audio/Audio.hpp"
#include <SDL2/SDL.h>

class Engine
{
    public:
        Engine();
        ~Engine();

        void mainLoop();

    private:
        void loadConfig();
        void setupSDL();
        void setupGL();
        void setupHighLevelSystems();

        void processEvents(const SDL_Event& event);
        void update();
        void render();

        void sendMessage(const Message& message);

    private:
        SDL_Window*      m_window = nullptr;
        SDL_GLContext    m_context;
        SDL_Event        m_event;

        chro::high_resolution_clock::time_point m_prevTime;
        chro::high_resolution_clock::time_point m_currTime;

        seconds    m_deltaTime;

        bool       m_running = true;
        bool       m_badassQuit = false;

        Input      m_input;
        Audio      m_audio;

        GameState::Ptr m_states[GameState::Type::Count];
        GameState* m_state = nullptr;
};
