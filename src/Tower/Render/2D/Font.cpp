#include "Font.hpp"
#include "Core/Log.hpp"
#include <fstream>

void Font::loadFromFile(const Path& path)
{
    std::ifstream file(path);

    if (!file.good())
    {
        Log::log(Log::Type::Error, "Could not load font %s\n", path.c_str());
        exit(EXIT_FAILURE);
    }

    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);

        if (line.substr(0,5) == "char ")
        {
            Glyph glyph;
            u32 index = 0;

            sscanf(line.c_str(),
                "char id=%d x=%f y=%f width=%f height=%f xoffset=%f yoffset=%f xadvance=%f page=0 chnl=0",
                &index,
                &glyph.x,
                &glyph.y,
                &glyph.width,
                &glyph.height,
                &glyph.xoffset,
                &glyph.yoffset,
                &glyph.xadvance
            );

            m_glyphs[index] = glyph;
        }
    }
}

Glyph Font::getGlyph(u8 index) const
{
    return m_glyphs[index];
}
