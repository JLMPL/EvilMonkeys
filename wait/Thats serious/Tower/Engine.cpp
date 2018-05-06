#include "Engine.hpp"
#include "Messages/MessageBus.hpp"
#include "GameState/GameMenu.hpp"
#include "GameState/GameHelp.hpp"
#include "GameState/GameSettings.hpp"
#include "GameState/GamePlaying.hpp"
#include "GameState/GameOver.hpp"
#include "Core/Log.hpp"
#include "Core/Config.hpp"
#include "Script/Lua.hpp"
#include "Resources/Resources.hpp"
#include "Render/Renderer3D.hpp"
#include "Render/Renderer2D.hpp"
#include <fstream>
#include <cmath>

Engine::Engine()
{
    loadConfig();
    setupSDL();
    setupGL();
    setupHighLevelSystems();

    m_states[GameState::Type::Menu] = GameState::Ptr(new GameMenu());
    m_states[GameState::Type::Help] = GameState::Ptr(new GameHelp());
    m_states[GameState::Type::Settings] = GameState::Ptr(new GameSettings());
    m_states[GameState::Type::Playing] = GameState::Ptr(new GamePlaying());
    m_states[GameState::Type::Over] = GameState::Ptr(new GameOver());

    m_state = m_states[GameState::Type::Menu].get();
    m_state->init();
}

Engine::~Engine()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Engine::loadConfig()
{
    std::ifstream file("config.ini");
    std::string line;

    std::getline(file, line);
    sscanf(line.c_str(), "DisplayWidth = %d;", &Config::Display::Width);

    std::getline(file, line);
    sscanf(line.c_str(), "DisplayHeight = %d;", &Config::Display::Height);

    std::getline(file, line);
    sscanf(line.c_str(), "DisplayFullscreen = %d;", &Config::Display::Fullscreen);
}

void Engine::setupSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        Log::log(Log::Type::Error, "SDL_Init failed!\n");
        exit(EXIT_FAILURE);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    u32 flags = SDL_WINDOW_OPENGL;

    if (Config::Display::Fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;
    else
        flags |= SDL_WINDOW_SHOWN;

    m_window = SDL_CreateWindow(
        "Tower",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        Config::Display::Width, Config::Display::Height,
        flags
    );

    if (!m_window)
    {
        Log::log(Log::Type::Error, "Window could not be created! %s\n", SDL_GetError());
        SDL_ClearError();
        exit(EXIT_FAILURE);
    }

    m_context = SDL_GL_CreateContext(m_window);

    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Engine::setupGL()
{
    GLenum error = glewInit();

    if(error != GLEW_OK)
    {
        Log::log(Log::Type::Error, "Could not initialize GLEW\n");
        exit(EXIT_FAILURE);
    }

    GL(glClearColor(0,0,0,1));
    GL(glEnable(GL_DEPTH_TEST));
    GL(glEnable(GL_CULL_FACE));
    GL(glCullFace(GL_BACK));
    GL(glEnable(GL_TEXTURE_2D));
    GL(glEnable(GL_TEXTURE_3D));
    GL(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));

    GL(glEnable(GL_POINT_SMOOTH));
    GL(glPointSize(2));
}

void Engine::setupHighLevelSystems()
{
    Renderer3D::Settings setts;

    gResources.init();
    gRenderer3D.init(setts);
    gRenderer2D.init();

    m_audio.init();

    m_currTime = std::chrono::high_resolution_clock::now();
}

void Engine::processEvents(const SDL_Event& event)
{
    if (event.type == SDL_QUIT)
        m_badassQuit = true;

    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_q &&
            event.key.keysym.mod == KMOD_LCTRL)
        {
            m_badassQuit = true;
        }
        else if (event.key.keysym.sym == SDLK_F2)
        {
            Config::Profiling::PhysicsDebugDraw = !Config::Profiling::PhysicsDebugDraw;
        }
    }
}

void Engine::sendMessage(const Message& message)
{
    m_state->sendMessage(message);
    m_audio.sendMessage(message);

    if (message.type == Message::Type::GameStateSet)
    {
        m_state->exit();
        m_state = m_states[message.state].get();
        m_state->init();
    }
    else if (message.type == Message::Type::GameStateExit)
    {
        m_badassQuit = true;
    }
}

void Engine::update()
{
    m_prevTime = m_currTime;
    m_currTime = std::chrono::high_resolution_clock::now();
    m_deltaTime = m_currTime - m_prevTime;

    m_input.update();
    m_audio.refresh(); //MAKE THAT A MESSAGE

    Message message;
    while (MessageBus::getNextMessage(message))
    {
        sendMessage(message);
    }

    m_state->update(m_deltaTime);
    gRenderer3D.update(m_deltaTime);
    gRenderer2D.update(m_deltaTime);
}

void Engine::render()
{
    gRenderer3D.render();
    gRenderer2D.render();

    SDL_GL_SwapWindow(m_window);
}

void Engine::mainLoop()
{
    while (m_running && !m_badassQuit)
    {
        while (SDL_PollEvent(&m_event))
        {
            processEvents(m_event);
        }


        update();
        render();
    }
}
