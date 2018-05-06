#include "Audio.hpp"
#include "Messages/MessageBus.hpp"
#include "Core/Log.hpp"
#include "Core/Config.hpp"
#include <SDL2/SDL_mixer.h>

Audio::~Audio()
{
    Mix_Quit();
}

void Audio::init()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        Log::log(Log::Type::Error, "Mixer initialization failed with error: %s\n", Mix_GetError());
    }

    m_newMagincia.loadFromFile("Music/new_magincia.mp3");
    m_newMagincia.setVolume(Config::Settings::MusicVolume);
    m_newMagincia.play();

    m_hurt.loadFromFile("Sounds/hurt.wav");
    m_hurt.setVolume(Config::Settings::EffectsVolume);

    m_swing.loadFromFile("Sounds/swing.wav");
    m_swing.setVolume(Config::Settings::EffectsVolume);
}

void Audio::sendMessage(const Message& message)
{
    if (message.type == Message::Type::SoundPlay)
    {
        switch (message.ivalue)
        {
            case 0:
                m_hurt.play();
                break;
            case 1:
                m_swing.play();
                break;
        }
    }
}

void Audio::refresh()
{
    m_newMagincia.setVolume(Config::Settings::MusicVolume);
    m_hurt.setVolume(Config::Settings::EffectsVolume);
    m_swing.setVolume(Config::Settings::EffectsVolume);
}
