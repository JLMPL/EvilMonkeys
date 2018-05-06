#pragma once
#include "Render/3D/Skybox.hpp"
#include "Core/Math3d.hpp"

class Weather
{
    public:
        Weather() = default;
        Weather(const Weather&) = delete;
        Weather& operator=(const Weather&) = delete;

        void init();
        void update(seconds deltaTime);

        void renderSkybox() const;

        vec3 getSunColor() const;
        vec3 getSunDirection() const;
        vec3 getAmbientColor() const;

    private:
        Skybox m_skybox;

        vec3 m_sunDirection;
        vec3 m_sunColor;
        vec3 m_sunAmbient;
};
