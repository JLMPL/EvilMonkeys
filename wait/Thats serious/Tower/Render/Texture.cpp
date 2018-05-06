#include "Texture.hpp"
#include "Core/Log.hpp"
#include <SDL2/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const char* path)
{
    loadFromFile(path);
}

Texture::~Texture()
{
    if(m_texture != 0)
        GL(glDeleteTextures(1, &m_texture));
}

void Texture::loadFromFile(const char* path)
{
    i32 w, h, channels;
    byte* image = stbi_load(path, &w, &h, &channels, 4);

    if (!image)
    {
        Log::log(Log::Type::Error, "Could not load texture \"%s\"\n", path);
        exit(EXIT_FAILURE);
    }

    m_width = w;
    m_height = h;

    GL(glGenTextures(1, &m_texture));
    GL(glBindTexture(GL_TEXTURE_2D, m_texture));

    GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image));

    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

    GL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8));

    GL(glGenerateMipmap(GL_TEXTURE_2D));
    GL(glBindTexture(GL_TEXTURE_2D, 0));

    stbi_image_free(image);
}

u32 Texture::getWidth() const
{
    return m_width;
}

u32 Texture::getHeight() const
{
    return m_height;
}

void Texture::bind() const
{
    GL(glBindTexture(GL_TEXTURE_2D, m_texture));
}

void Texture::unbind() const
{
    GL(glBindTexture(GL_TEXTURE_2D, 0));
}
