#pragma once
#include "Core/Math3d.hpp"
#include <SDL2/SDL.h>

class Input
{
    public:
        void update();

    private:
        void sendKeyMessage(i32 key);
        void checkKey(i32 key);

    private:
        const Uint8* m_keys = nullptr;

        vec2i m_mouseRelative;
};
