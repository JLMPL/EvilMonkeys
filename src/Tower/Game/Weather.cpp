#include "Weather.hpp"

void Weather::init()
{
    m_sunDirection = math::normalize(vec3(-1.f,-0.75f,-1.f));

    m_sunColor = vec3(3.f,3.f,3.f);
    m_sunAmbient = vec3(0.25f,0.25f,0.25f);

    // m_sunColor = vec3(0.01f,0.01f,0.01f);
    // m_sunAmbient = vec3(0.f,0.f,0.f);

    // m_skybox.init("hills");
}

void Weather::update(seconds deltaTime)
{
    // m_time += deltaTime * 0.1f;

    // m_sunDirection = math::normalize(vec3(cos(m_time), -0.75f, sin(m_time)));

    // m_sunColor = vec3(sin(m_time) + 1.f);
    // m_sunAmbient = vec3((sin(m_time) + 1.f) * 0.25f);
}

void Weather::renderSkybox() const
{
    m_skybox.render(true);
}

vec3 Weather::getSunColor() const
{
    return m_sunColor;
}

vec3 Weather::getSunDirection() const
{
    return m_sunDirection;
}

vec3 Weather::getAmbientColor() const
{
    return m_sunAmbient;
}
