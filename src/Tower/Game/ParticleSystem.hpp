#pragma once
#include "Particle.hpp"
#include <vector>

class DebugRenderer;
struct Message;

class ParticleSystem
{
    public:
        ParticleSystem() = default;
        ParticleSystem(const ParticleSystem&) = delete;
        ParticleSystem& operator=(const ParticleSystem&) = delete;

        void init();
        void addParticle(const vec3& pos, const vec3& dir, const vec3& color, float velocity, float weight, seconds lifetime = seconds(5));
        void update(seconds deltaTime);

        void sendMessage(const Message& message);

    private:
        vec3 m_gravity;
        std::vector<Particle> m_particles;
};
