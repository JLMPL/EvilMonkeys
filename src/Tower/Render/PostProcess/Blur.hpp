#pragma once
#include "../ScreenQuad.hpp"
#include "../Framebuffer.hpp"
#include "../Shader.hpp"

class PostProcessBlur
{
    public:
        enum class KernelSize
        {
            Big,
            Small
        };

        PostProcessBlur() = default;

        void init(float factor, KernelSize kernelSize = KernelSize::Big);

        GLuint process(GLuint texture);

    private:
        void horizontalPass();
        void verticalPass();

    private:
        struct Pass
        {
            Framebuffer fbo;
            Shader      shader;
            GLuint      texture;
        };

        KernelSize    m_kernelSize;

        ScreenQuad    m_screenQuad;

        GLuint        m_original;

        Pass          m_horizontal;
        Pass          m_vertical;
};
