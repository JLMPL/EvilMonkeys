#pragma once
#include "Core/Types.hpp"
#include <SDL2/SDL_mixer.h>
#include <string>

class Sound
{
    public:
        Sound() = default;
        ~Sound();

        void loadFromFile(const Path& path);

        void play();

        void setVolume(u32 vol);

    private:
        Mix_Chunk* m_chunk = nullptr;
        u32 m_volume = MIX_MAX_VOLUME;
};
