#pragma once
#include "Core/Types.hpp"
#include <memory>

struct Glyph
{
    f32 x,y;
    f32 width, height;
    f32 xoffset, yoffset;
    f32 xadvance;
};

class Font
{
    public:
        using Ptr = std::shared_ptr<Font>;

        Font() = default;
        ~Font() = default;

        void loadFromFile(const Path& path);

        Glyph getGlyph(u8 index) const;

    private:
        Glyph m_glyphs[128];
};
