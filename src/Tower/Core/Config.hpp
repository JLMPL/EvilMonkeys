#pragma once
#include "Core/Types.hpp"
#include <SDL2/SDL.h>

namespace Config
{

namespace Display
{
    extern u32 Width;
    extern u32 Height;
    extern bool Fullscreen;
}

namespace Profiling
{
    extern bool ProfileInfo;
    extern bool PhysicsDebugDraw;
}

namespace Render
{
}

namespace Settings
{
    extern i32 MusicVolume;
    extern i32 EffectsVolume;
}

namespace Controls
{
    constexpr i32 Up     = SDL_SCANCODE_UP;
    constexpr i32 Down   = SDL_SCANCODE_DOWN;
    constexpr i32 Left   = SDL_SCANCODE_LEFT;
    constexpr i32 Right  = SDL_SCANCODE_RIGHT;
    constexpr i32 Jump   = SDL_SCANCODE_LALT;
    constexpr i32 Use    = SDL_SCANCODE_E;
    constexpr i32 Block  = SDL_SCANCODE_Q;
    constexpr i32 Sprint = SDL_SCANCODE_LSHIFT;
    constexpr i32 Mode   = SDL_SCANCODE_SPACE;
    constexpr i32 Prep   = SDL_SCANCODE_LCTRL;
    constexpr i32 Return = SDL_SCANCODE_RETURN;
    constexpr i32 Exit   = SDL_SCANCODE_ESCAPE;
    constexpr i32 Cheat  = SDL_SCANCODE_F10;
}

}
