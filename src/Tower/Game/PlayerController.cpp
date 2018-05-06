#include "PlayerController.hpp"
#include "Messages/MessageBus.hpp"
#include "World.hpp"
#include "Actor.hpp"
#include "Actor/States.hpp"
#include "Render/3D/Camera.hpp"
#include "Core/Config.hpp"

void PlayerController::init(Actor* actor, World* world, Camera* camera)
{
    m_world = world;
    m_actor = actor;
    m_camera = camera;

    initStates();
    setState(ActorState::Type::Idle);

    m_actor->_focus = m_world->getEntityById(1);
}

void PlayerController::initStates()
{
    m_states[ActorState::Type::Idle]     = std::unique_ptr<PlayerIdle>(new PlayerIdle());
    m_states[ActorState::Type::Movement] = std::unique_ptr<PlayerMovement>(new PlayerMovement());
    m_states[ActorState::Type::Attack]   = std::unique_ptr<ActorAttack>(new ActorAttack());
    m_states[ActorState::Type::Block]    = std::unique_ptr<ActorBlock>(new ActorBlock());
    m_states[ActorState::Type::Dead]     = std::unique_ptr<ActorDead>(new ActorDead());
    m_states[ActorState::Type::Pain]     = std::unique_ptr<ActorPain>(new ActorPain());
    m_states[ActorState::Type::Dodge]    = std::unique_ptr<ActorDodge>(new ActorDodge());
}

void PlayerController::sendMessage(const Message& message)
{
    if (message.recieverEntity == m_actor->_id)
    {
        if (message.type == Message::Type::ActorSetState)
        {
            i32 state = message.state;
            setState(ActorState::Type(state), message.stateFlags);
        }
        else if (message.type == Message::Type::ActorDamage)
        {
            if (m_state->isVulnerable())
            {
                m_actor->_health -= message.ivalue;

                Message message(Message::Type::InterfaceSetHealth);
                message.ivalue = m_actor->_health;
                MessageBus::queueMessage(message);

                if (m_actor->isDead())
                    setState(ActorState::Type::Dead, 0);
                else
                    setState(ActorState::Type::Pain, 0);
            }
        }
    }

    m_state->sendMessage(message);
}

void PlayerController::update(seconds deltaTime)
{
    if (m_actor)
    if (!m_actor->isDead())
    {
        if (m_actor->_focus)
        {
            m_actor->_dir = math::normalize(m_actor->_focus->_pos - m_actor->_pos);
            m_actor->_yaw = atan2(m_actor->_dir.x, m_actor->_dir.z);
        }

        {
            Message message(Message::Type::ParticlesSpawn);
            message.particles.num = 1;

            vec4 pos = mat4(m_actor->_attachTrs[Actor::AttachSpot::RightHand] * glm::translate(vec3(0,4,0)))[3];

            message.particles.pos.x = pos.x;
            message.particles.pos.y = pos.y;
            message.particles.pos.z = pos.z;

            message.particles.dir.x = 0;
            message.particles.dir.y = 0;
            message.particles.dir.z = 0;

            message.particles.color.x = 1;
            message.particles.color.y = 1;
            message.particles.color.z = 1;

            message.particles.velocity = 0;
            message.particles.weight = 0;

            message.particles.life = 0.25;

            MessageBus::queueMessage(message);
        }
    }

    m_state->update(deltaTime);

    vec3 front = m_actor->_focus->_pos + vec3(0,8,0);
    m_camera->setCenter(front);

    vec3 back = m_actor->_pos - m_actor->_dir * 12.f;
    back += vec3(0,10,0);

    vec3 side = math::normalize(math::cross(m_actor->_dir, math::vecY));
    back += side * 10.f;

    m_camera->setEye(back);
}

void PlayerController::setState(ActorState::Type state, i32 flags)
{
    m_state = m_states[state].get();
    m_state->enter(m_actor, flags);
}
