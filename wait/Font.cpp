#include "Font.hpp"
#include "Core/Log.hpp"
#include <fstream>

Font::~Font()
{
    // TTF_CloseFont(m_font); SEGFAULT
}

void Font::loadFromFile(const Path& path, u32 size)
{
    m_font = TTF_OpenFont(path.c_str(), size);
}

TTF_Font* Font::get() const
{
    return m_font;
}
