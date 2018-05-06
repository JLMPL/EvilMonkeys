#pragma once
#include "Texture.hpp"
#include "Shader.hpp"
#include "Core/OrderlessArray.hpp"
#include "Core/Math3d.hpp"

class Rectangle;
class Text;

class Renderer2D
{
    public:
        Renderer2D() = default;
        ~Renderer2D() = default;

        void init();
        void update(seconds deltaTime);
        void render();

        void registerRectangle(const Rectangle* rect);
        void unregisterRectangle(const Rectangle* rect);
        void registerText(const Text* text);
        void unregisterText(const Text* text);

        void clearAllData();

    private:
        struct Shaders
        {
            Shader rectangle;
            Shader text;
        };

        Shaders m_shaders;
        mat4 m_proj;

        OrderlessArray<const Rectangle*, 128u> m_rectangles;
        OrderlessArray<const Text*, 128u> m_texts;

        Texture m_textture;
};

extern Renderer2D gRenderer2D;
