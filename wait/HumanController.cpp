#include "HumanController.hpp"
#include "World.hpp"
#include "Core/Types.hpp"
#include "Core/Config.hpp"
#include "Core/Convert.hpp"
#include <SDL2/SDL.h>

constexpr float g_combatRangeMin = 3.f;
constexpr float g_combatRangeMax = 12.f;
constexpr float g_strafeDuration = 0.5f;

HumanController::HumanController(Actor* character)
{
    m_actor = character;
    m_world = m_actor->m_world;

    setMindState(MindState::Idle);

    enterIdle();
}

void HumanController::processEvents(const SDL_Event& event)
{
}

void HumanController::preUpdate(float deltaTime)
{
    m_timer += deltaTime;

    if (m_actor->isDead())
        enterDeath();

    switchStates(deltaTime);
}

void HumanController::postUpdate(float deltaTime)
{
}

void HumanController::sendEvent(const WorldEvent& worldEvt)
{
    switch (worldEvt.type)
    {
        case WorldEvent::Type::CharDamage:
            onDamage(worldEvt);
            break;
        case WorldEvent::Type::CharDeath:
            onDeath(worldEvt);
            break;
        default:break;
    }
}

void HumanController::setMindState(MindState state)
{
    m_mindState = state;
}

void HumanController::updateIdle(float deltaTime)
{
    switch (m_mindState)
    {
        case MindState::Follow:
        {
            float distance = glm::distance(m_focus->getPosition(), m_actor->getPosition());

            if (distance > 20.f)
                enterMovement(Strafe::None);
        }
        break;
        case MindState::Combat:
        {
            if (m_focus)
            {
                if (m_focus->as<Actor>()->isDead())
                {
                    m_focused = false;
                    m_focus = nullptr;
                    enterIdle();
                }
            }

            if (m_focused && m_timer >= 1.f)
            {
                int32 num = int32(NextCombatAction::NumActions) + 2;
                m_nextAction = NextCombatAction((rand() % num) + 1);

                switch (m_nextAction)
                {
                    case NextCombatAction::Idle:
                        enterIdle();
                        break;
                    case NextCombatAction::Attack:
                        enterPunch();
                        break;
                    case NextCombatAction::StrafeLeft:
                        enterMovement(Strafe::Left);
                        break;
                    case NextCombatAction::StrafeRight:
                        enterMovement(Strafe::Right);
                        break;
                    case NextCombatAction::Block:
                        enterBlock();
                        break;

                    default:
                        enterPunch();
                    break;
                }

                m_timer = 0.f;
            }

            if (m_focused)
            {
                float distance = glm::distance(m_focus->getPosition(), m_actor->getPosition());

                if (distance > g_combatRangeMax)
                {
                    enterMovement(Strafe::None);
                }
                else if (distance < g_combatRangeMin)
                {
                    enterDodge();
                }
                else
                {
                    m_actor->m_dir = glm::normalize(m_focus->getPosition() - m_actor->getPosition());
                    m_actor->m_yaw = atan2(m_actor->m_dir.x, m_actor->m_dir.z);
                }
            }
        }
        break;
    }

    m_actor->m_pos = m_actor->m_physCont.getPosition();
}

void HumanController::enterMovement(Strafe strafe)
{
    m_strafe = strafe;

    switch (m_strafe)
    {
        case Strafe::None:
            m_actor->setAnimation("Guy_run.x");
            break;
        case Strafe::Left:
            m_actor->setAnimation("Guy_strafeLeft.x");
            break;
        case Strafe::Right:
            m_actor->setAnimation("Guy_strafeRight.x");
            break;
    }

    m_actor->m_physCont.lowFriction();

    m_timer = 0.f;
    m_actor->m_state = Actor::State::Movement;
}

void HumanController::updateMovement(float deltaTime)
{
    m_actor->m_dir = glm::normalize(m_focus->getPosition() - m_actor->getPosition());
    m_actor->m_yaw = atan2(m_actor->m_dir.x, m_actor->m_dir.z);

    switch (m_mindState)
    {
        case MindState::Follow:
            updateMovementFollow(deltaTime);
            break;
        case MindState::Combat:
            updateMovementCombat(deltaTime);
            break;
    }

    // m_actor->m_physCont.movePosToBody(m_actor->m_pos);
    m_actor->m_pos = m_actor->m_physCont.getPosition();
}

void HumanController::updateMovementFollow(float deltaTime)
{
    float distance = glm::distance(m_focus->getPosition(), m_actor->getPosition());

    auto result = m_actor->m_physicsWorld->rayTest(
        m_actor->m_pos + vec3f(0.f,1.f,0.f),
        m_actor->m_pos + vec3f(0.f,1.f,0.f) + m_actor->m_dir * 2.f
    );

    if (result.hasHit())
    {
        auto ledge = m_actor->findLedge();

        if (ledge.pos != g_vec0)
            enterClimb(ledge);
    }

    if (distance < 20.f)
        enterIdle();
    else
    {
        m_actor->m_physCont.setPlanarVelocity(vec2f(
            m_actor->m_dir.x * m_actor->m_speed,
            m_actor->m_dir.z * m_actor->m_speed
        ));
    }
}

void HumanController::updateMovementCombat(float deltaTime)
{
    float distance = glm::distance(m_focus->getPosition(), m_actor->getPosition());

    switch (m_strafe)
    {
        case Strafe::None:
        {
            if (distance > 50.f)
            {
                m_focused = false;
                m_focus = nullptr;

                enterIdle();
            }
            else if (distance < g_combatRangeMax)
            {
                enterIdle();
            }
            else if (distance < g_combatRangeMin)
            {
                enterDodge();
            }
            else
            {
                auto result = m_actor->m_physicsWorld->rayTest(
                    m_actor->m_pos + vec3f(0.f,1.f,0.f),
                    m_actor->m_pos + vec3f(0.f,1.f,0.f) + m_actor->m_dir * 2.f
                );

                if (result.hasHit())
                {
                    auto ledge = m_actor->findLedge();

                    if (ledge.pos != g_vec0)
                        enterClimb(ledge);
                }

                m_actor->m_physCont.setPlanarVelocity(vec2f(
                    m_actor->m_dir.x * m_actor->m_speed,
                    m_actor->m_dir.z * m_actor->m_speed
                ));

                // m_actor->m_physCont.move(m_actor->m_dir * m_actor->m_speed * deltaTime);
            }
        }
        break;
        case Strafe::Left:
        {
            vec3f dire = glm::cross(m_actor->m_dir, g_vecY);
            // m_actor->m_physCont.move(-dire * m_actor->m_speed * deltaTime);

            m_actor->m_physCont.setPlanarVelocity(vec2f(
                -dire.x * m_actor->m_speed,
                -dire.z * m_actor->m_speed
            ));

            if (m_timer >= g_strafeDuration)
                enterIdle();
        }
        break;
        case Strafe::Right:
        {
            vec3f dire = glm::cross(m_actor->m_dir, g_vecY);
            // m_actor->m_physCont.move(dire * m_actor->m_speed * deltaTime);

            m_actor->m_physCont.setPlanarVelocity(vec2f(
                dire.x * m_actor->m_speed,
                dire.z * m_actor->m_speed
            ));

            if (m_timer >= g_strafeDuration)
                enterIdle();
        }
        break;
    }
}

void HumanController::onDamage(const WorldEvent& worldEvt)
{
    Base::onDamage(worldEvt);
    setMindState(MindState::Combat);
}

void HumanController::onDeath(const WorldEvent& worldEvt)
{
    // if (!m_actor->isDead())
    // {
    //     setMindState(MindState::Combat);

    //     m_focus = m_world->getEntityById(worldEvt.values[0].i);
    //     m_focused = true;
    // }
}

// YOU COULDN'T COMMIT TO BEING 100% ASIAN
