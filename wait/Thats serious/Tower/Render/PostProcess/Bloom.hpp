#pragma once
#include "../ScreenQuad.hpp"
#include "../Framebuffer.hpp"
#include "../Shader.hpp"
#include "Blur.hpp"

class PostProcessBloom
{
    public:
        PostProcessBloom() = default;

        void init();

        GLuint process(GLuint texture);

    private:
        void lumaPass();
        void addPass();

    private:
        struct Pass
        {
            Framebuffer fbo;
            Shader      shader;
            GLuint      texture;
        };

        PostProcessBlur m_blur;

        ScreenQuad    m_screenQuad;

        GLuint        m_original;
        GLuint        m_working;

        Pass          m_luma;
        Pass          m_add;
};
