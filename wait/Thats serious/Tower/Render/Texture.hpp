#pragma once
#include "Core/Types.hpp"
#include "Render/OpenGL.hpp"
#include <stdint.h>
#include <memory>

class Texture
{
    public:
        using Ptr = std::shared_ptr<Texture>;

        Texture() = default;
        Texture(const char* path);
        ~Texture();

        void loadFromFile(const char* path);

        u32 getWidth() const;
        u32 getHeight() const;

        void bind() const;
        void unbind() const;

    private:
        GLuint m_texture = 0;
        u32 m_width = 0;
        u32 m_height = 0;
};
