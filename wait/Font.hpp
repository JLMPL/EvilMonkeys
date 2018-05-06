#pragma once
#include "Core/Types.hpp"
#include <SDL2/SDL_ttf.h>
#include <string>
#include <memory>

class Font
{
    public:
        using Ptr = std::shared_ptr<Font>;

        Font() = default;
        ~Font();

        void loadFromFile(const Path& path, u32 size = 24);

        TTF_Font* get() const;

    private:
        TTF_Font* m_font;
};
