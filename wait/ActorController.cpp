#include "ActorController.hpp"
#include "Actor.hpp"
#include "World.hpp"
#include "Core/Convert.hpp"

constexpr float g_weaponRange = 12.f;

ActorController::ActorController(Actor* character)
{
    m_actor = character;
}

void ActorController::switchStates(float deltaTime)
{
    switch (m_actor->m_state)
    {
        case Actor::State::Idle:
            updateIdle(deltaTime);
            break;
        case Actor::State::Movement:
            updateMovement(deltaTime);
            break;
        case Actor::State::Jump:
            updateJump(deltaTime);
            break;
        case Actor::State::Climb:
            updateClimb(deltaTime);
            break;
        case Actor::State::Punch:
            updatePunch(deltaTime);
            break;
        case Actor::State::Dodge:
            updateDodge(deltaTime);
            break;
        case Actor::State::Block:
            updateBlock(deltaTime);
            break;
        case Actor::State::GetHit:
            updateGetHit(deltaTime);
            break;
        case Actor::State::Death:
            updateDeath(deltaTime);
            break;
    }
}

void ActorController::keepOnGround()
{
    if (m_actor->m_state != Actor::State::Jump)
    {
        auto shape = btSphereShape(1.f);

        auto pos = m_actor->m_physCont.getPosition();

        vec3f rendezvous = pos + vec3f(0.f,3.f,0.f);
        auto result = m_world->m_physicsWorld.rayTest(rendezvous, pos + vec3f(0.f,-0.5f,0.f));

        if (result.hasHit())
        {
            vec3f point = btToGlm(result.m_hitPointWorld);

            if (glm::distance(rendezvous, point) > 3.05f)
            {
                m_actor->m_physCont.setVerticalVelocity(-5.f);
            }
        }
    }
}

void ActorController::moveAlongGround(const vec3f& originalDir)
{
    m_actor->m_yaw = atan2(m_actor->m_dir.x, m_actor->m_dir.z);

    vec3f side = glm::normalize(glm::cross(originalDir, g_vecY));

    // g_renderer.m_debugRenderer.addLine(m_actor->m_pos, m_actor->m_pos + side * 10.f, g_vecX);

    auto result = m_actor->m_physicsWorld->rayTest(m_actor->m_pos + g_vecY, m_actor->m_pos - g_vecY);

    if (result.hasHit())
    {
        vec3f dir = glm::cross(side, btToGlm(result.m_hitNormalWorld));
        dir = -glm::normalize(dir);

        // g_renderer.m_debugRenderer.addLine(m_actor->m_pos, m_actor->m_pos + dir * 10.f, g_vecX);

        m_actor->m_physCont.setVelocity(dir * m_actor->m_speed);
        m_actor->m_physCont.lowFriction();
    }
}

void ActorController::updateIdle(float deltaTime) {}

void ActorController::enterMovement()
{
    m_actor->m_state = Actor::State::Movement;
}

void ActorController::updateMovement(float deltaTime) {}

void ActorController::enterIdle()
{
    if (!m_focused)
        m_actor->setAnimation("Guy.x");
    else
        m_actor->setAnimation("Guy_combat.x");

    m_actor->m_physCont.highFriction();

    m_actor->m_state = Actor::State::Idle;
}

bool shit = false;

void ActorController::enterPunch()
{
    m_actor->setAnimation("Guy_punch.x", 0.01f, false, &m_punchNow, 0.5f);
    m_actor->m_state = Actor::State::Punch;
    m_timer = 0.f;

    shit = false;
}

void ActorController::updatePunch(float deltaTime)
{
    if (m_punchNow && m_focus)
    {
        vec3f difec = m_focus->getPosition() - m_actor->getPosition();

        if (glm::length(difec) <= g_weaponRange)
        {
            WorldEvent event(WorldEvent::Type::CharDamage, m_actor->getId(), m_focus->getId());
            event.values[0].i = 1;

            m_actor->m_world->sendEvent(event);
        }

        m_punchNow = false;
        shit = true;
    }

    if (!shit)
        moveAlongGround(m_actor->m_dir * 2.5f);
    else
        m_actor->m_physCont.highFriction();


    if (m_actor->m_animMesh.isFinished())
        enterIdle();

    m_actor->m_pos = m_actor->m_physCont.getPosition();
}


void ActorController::enterDeath()
{
    m_actor->setAnimation("Guy_death.x", 0.2f, false);

    WorldEvent event(WorldEvent::Type::CharDeath, m_actor->getId(), -1);
    event.values[0].i = m_focus->getId();

    m_actor->m_world->sendEvent(event);

    m_actor->m_physCont.highFriction();

    m_actor->m_state = Actor::State::Death;
}

void ActorController::updateDeath(float deltaTime)
{
    //IT'S DEATH WHAT DID YOU EXPECT?
}

void ActorController::enterJump(float speed)
{
    if (speed > 0.f)
    {
        m_actor->m_physCont.setVelocity(vec3f(
            m_actor->m_dir.x * speed,
            12.f,
            m_actor->m_dir.z * speed
        ));
    }

    m_actor->m_physCont.lowFriction();

    m_actor->setAnimation("Guy_launch.x", 0.05f, false);
    m_timer = 0.f;

    m_actor->m_state = Actor::State::Jump;
}

void ActorController::updateJump(float deltaTime)
{
    if (m_actor->m_physCont.checkOnGround() && m_timer >= 0.5f)
        enterIdle();

    // m_actor->m_physCont.move(m_actor->m_dir * m_actor->m_speed * deltaTime);

    auto ledge = m_actor->findLedge();

    if (ledge.pos != g_vec0)
        enterClimb(ledge);

    m_actor->m_pos = m_actor->m_physCont.getPosition();
}

void ActorController::enterClimb(LedgeData ledge)
{
    m_ledge = ledge;

    if (m_ledge.height < g_lowHeightThreshold)
    {
        m_actor->setAnimation("Guy_climbLow.x", 0.05f, false);
    }
    else if (m_ledge.height >= g_lowHeightThreshold &&
             m_ledge.height < g_midHeightThreshold)
    {
        m_actor->setAnimation("Guy_climbMid.x", 0.05f, false);
    }
    else
    {
        m_actor->setAnimation("Guy_climbHigh.x", 0.0f, false);
    }

    m_actor->m_state = Actor::State::Climb;
}

void ActorController::updateClimb(float deltaTime)
{
    m_actor->m_physCont.setPosition(m_ledge.pos - m_ledge.normal * 0.5f);
    m_actor->m_yaw = atan2(-m_ledge.normal.x, -m_ledge.normal.z);

    if (m_actor->m_animMesh.isFinished())
        enterIdle();
}

void ActorController::enterGetHit()
{
    m_actor->setAnimation("Guy_hit.x", 0.1f, false);

    for (auto i = 0; i < 250; i++)
    {
        float x   = float((rand() % 2001) - 1000) / 1000.f;
        float y   = float((rand() % 2001) - 1000) / 1000.f;
        float z   = float((rand() % 2001) - 1000) / 1000.f;
        float vel = float(rand() % 10001) / 1000.f;
        float we  = float(rand() % 15001) / 1000.f;

        vec3f dir = vec3f(x,y,z) + m_actor->m_dir;

        m_world->getParticleSystem().addParticle(m_actor->m_pos + vec3f(0.f,6.f,0.f), dir, vec3f(1.f,0.f,0.f), vel, we);
    }

    m_timer = 0.f;
    m_actor->m_state = Actor::State::GetHit;
}

void ActorController::updateGetHit(float deltaTime)
{
    if (m_actor->m_animMesh.isFinished())
        enterIdle();

    m_actor->m_pos = m_actor->m_physCont.getPosition();

}

void ActorController::enterDodge()
{
    m_actor->setAnimation("Guy_dodge.x", 0.075, false);

    m_timer = 0.f;
    m_actor->m_state = Actor::State::Dodge;
}

void ActorController::updateDodge(float deltaTime)
{
    if (m_actor->m_animMesh.isFinished())
    {
        enterIdle();
    }

    moveAlongGround(-m_actor->m_dir * m_actor->m_speed * 1.5f);
    m_actor->m_pos = m_actor->m_physCont.getPosition();
}

void ActorController::enterBlock()
{
    m_actor->setAnimation("Guy_block.x", 0.001f, false);

    if (m_focus)
    if (m_focus->as<Actor>()->m_state == Actor::State::Punch)
    {
        for (auto i = 0; i < 50; i++)
        {
            float x   = float((rand() % 2001) - 1000) / 1000.f;
            float y   = float((rand() % 2001) - 1000) / 1000.f;
            float z   = float((rand() % 2001) - 1000) / 1000.f;
            float vel = float(rand() % 10001) / 1000.f;
            float we  = float(rand() % 5001) / 1000.f;

            mat4f where =
                m_actor->m_transform *
                glm::translate(vec3f(0.f,2.5f,0.f)) *
                (*m_actor->m_animMesh.getJointTransform("Rig_RightHandAttach"));

            m_world->getParticleSystem().addParticle(vec3f(where[3]), vec3f(x,y,z), vec3f(1.f,1.f,0.6f), vel, we);
        }
    }

    m_timer = 0.f;
    m_actor->m_state = Actor::State::Block;
}

void ActorController::updateBlock(float deltaTime)
{
    if (m_actor->m_animMesh.isFinished())
        enterIdle();

    m_actor->m_physCont.setPosition(m_actor->m_pos);
}

//EVENTS

void ActorController::onDamage(const WorldEvent& worldEvt)
{
    if (!m_actor->isDead())
    {
        if (m_actor->m_state != Actor::State::Dodge &&
            m_actor->m_state != Actor::State::Block)
        {
            if (!m_actor->m_player)
            {
                uint8 ch = rand() % 4;

                if (ch == 0)
                    enterDodge();
                if (ch == 1)
                    enterBlock();
                else
                {
                    m_actor->m_health--;
                    m_actor->m_label.setString(std::to_string(m_actor->m_health));
                    enterGetHit();
                }
            }
            else
            {
                m_actor->m_health--;
                m_actor->m_label.setString(std::to_string(m_actor->m_health));
                enterGetHit();
            }
        }

        if (!m_actor->m_player)
        {
            m_focus = m_actor->m_world->getEntityById(worldEvt.sender);
            m_focused = true;
        }
    }
}

//GETTER

uint32 ActorController::getActorsId()
{
    return m_actor->m_id;
}
