#include "Cubemap.hpp"
#include <stb/stb_image.h>

Cubemap::~Cubemap()
{
    GL(glDeleteTextures(1, &m_cubemap));
}

void Cubemap::init(u32 size, GLenum internal, GLenum basic, GLenum type)
{
    GL(glGenTextures(1, &m_cubemap));
    GL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap));

    for (auto i = 0; i < 6; i++)
    {
        GL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal, size, size, 0, basic, type, nullptr));
    }

    GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void Cubemap::initFromFiles(const std::string* files, bool filesHavePostfixes)
{
    byte* images[6];

    i32 w, h, channels;

    if (!filesHavePostfixes)
    {
        for (auto i = 0; i < 6; i++)
        {
            images[i] = stbi_load(std::string("Textures/" + files[i]).c_str(), &w, &h, &channels, 4);
        }
    }
    else
    {
        std::string suffixes[] =
        {
            "_ft.png",
            "_bk.png",
            "_up.png",
            "_dn.png",
            "_rt.png",
            "_lf.png"
        };

        for (auto i = 0; i < 6; i++)
        {
            images[i] = stbi_load(std::string("Textures/" + files[0] + suffixes[i]).c_str(), &w, &h, &channels, 4);
        }
    }

    GL(glGenTextures(1, &m_cubemap));
    GL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap));

    for (auto i = 0; i < 6; i++)
    {
        GL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i]));

        stbi_image_free(images[i]);
    }

    GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    GL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
    GL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void Cubemap::generateMips()
{
    GL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
}

void Cubemap::bind() const
{
    GL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap));
}

void Cubemap::unbind() const
{
    GL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

GLuint Cubemap::getId() const
{
    return m_cubemap;
}
