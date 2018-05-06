#include "ParticleSystem.hpp"
#include "Messages/MessageBus.hpp"
#include "Render/Renderer3D.hpp"
#include "Core/Random.hpp"
#include <algorithm>

void ParticleSystem::init()
{
    m_gravity = vec3(0.f,-1.f,0.f);
}

void ParticleSystem::addParticle(const vec3& pos, const vec3& dir, const vec3& color, float velocity, float weight, seconds lifetime)
{
    using namespace chli;

    Particle part;
    part.pos      = pos;
    part.dir      = dir;
    part.color    = color;
    part.velocity = velocity;
    part.weight   = weight;
    part.life     = 0s;
    part.lifetime = lifetime;

    m_particles.push_back(part);
}

void ParticleSystem::sendMessage(const Message& message)
{
    if (message.type == Message::Type::ParticlesSpawn)
    {
        for (u32 i = 0; i < message.particles.num; i++)
        {
            f32 varx = Random::inRange(-1.f, 1.f);
            f32 vary = Random::inRange(-1.f, 1.f);
            f32 varz = Random::inRange(-1.f, 1.f);

            f32 vel = Random::inRange(-2.f, 2.f);

            vec3 dir = math::normalize(vec3(
                message.particles.dir.x + varx,
                message.particles.dir.y + vary,
                message.particles.dir.z + varz
            ));

            addParticle(
                {message.particles.pos.x, message.particles.pos.y, message.particles.pos.z},
                dir,
                {message.particles.color.x, message.particles.color.y, message.particles.color.z},
                message.particles.velocity + vel,
                message.particles.weight,
                seconds(message.particles.life)
            );
        }
    }
}

void ParticleSystem::update(seconds deltaTime)
{
    using namespace chli;

    m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(),
    [](const Particle& part)
    {
        return part.life > part.lifetime;
    }),
    m_particles.end());

    for (auto& p : m_particles)
    {
        p.life += deltaTime;
        p.prevPos = p.pos;
        p.dir += p.weight * m_gravity * deltaTime.count();
        p.pos += p.dir * p.velocity * deltaTime.count();

        gRenderer3D.addPoint(p.pos, p.color);
        gRenderer3D.addLine(p.prevPos, p.pos, p.color);
    }
}
