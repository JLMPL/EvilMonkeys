#include "PlayerController.hpp"
#include "World.hpp"
#include "Input/Input.hpp"
#include "Core/Types.hpp"
#include "Core/Config.hpp"
#include "Core/Convert.hpp"
#include <SDL2/SDL.h>

PlayerController::PlayerController(Actor* character)
{
    m_actor = character;
    m_world = m_actor->m_world;

    m_camera.init(&m_cameraTransform, &m_world->m_physicsWorld);
    g_renderer.m_sceneRenderer.setActiveCamera(&m_camera);

    enterIdle();
}

void PlayerController::processEvents(const SDL_Event& event)
{
}

void PlayerController::pickClosestFocus()
{
    auto ents = m_world->getEntitiesInRange(m_actor->m_pos, 200.f);
    float closest = 200.f;

    if (!ents.empty())
    for (auto i = 0; i < ents.size(); i++)
    {
        float dist = glm::distance(m_actor->m_pos, ents[i]->getPosition());

        vec3f theDir = glm::normalize(ents[i]->getPosition() - m_actor->getPosition());

        if (dist < closest && !ents[i]->as<Actor>()->isDead())
        {
            m_focus = ents[i];
            closest = dist;
        }
    }

    if (!m_focus)
        m_focused = false;
}

void PlayerController::preUpdate(float deltaTime)
{
    m_timer += deltaTime;

    if (m_actor->isDead())
        enterDeath();

    switchStates(deltaTime);
}

void PlayerController::updateCamera()
{
    m_cameraYaw -= float(Input::getMouseRelativePos().x) / 500.f;
    m_cameraPitch += float(Input::getMouseRelativePos().y) / 500.f;

    if (m_cameraPitch > HALF_PI - 0.2f)
        m_cameraPitch = HALF_PI - 0.2f;
    if (m_cameraPitch < -HALF_PI + 0.2f)
        m_cameraPitch = -HALF_PI + 0.2f;

    float fullRad = M_PI*2;

    if (m_cameraYaw < 0)
        m_cameraYaw += fullRad;
    if (m_cameraYaw > fullRad)
        m_cameraYaw -= fullRad;

    auto tr = m_actor->m_transform * (*m_actor->m_animMesh.getJointTransform("Rig_Ass"));
    m_cameraPos = vec3f(tr[3]) + vec3f(0.f,3.f,0.f);

    m_cameraTransform =
        glm::translate(m_cameraPos) *
        glm::rotate(m_cameraYaw, g_vecY) *
        glm::rotate(m_cameraPitch, g_vecX);

    m_camera.update();
}

void PlayerController::postUpdate(float deltaTime)
{
    updateCamera();
    keepOnGround();
}

void PlayerController::sendEvent(const WorldEvent& worldEvt)
{
    switch (worldEvt.type)
    {
        case WorldEvent::Type::CharDamage:
            onDamage(worldEvt);
            break;
        default:break;
    }
}

void PlayerController::enterIdle()
{
    Base::enterIdle();

    if (m_focused)
        m_actor->setAnimation("Guy_combat.x");
    else
        m_actor->setAnimation("Guy.x");

    m_actor->m_physCont.highFriction();
}

void PlayerController::updateFreeIdle(float deltaTime)
{
    if (Input::isSet(Input::Up) ||
        Input::isSet(Input::Down) ||
        Input::isSet(Input::Left) ||
        Input::isSet(Input::Right))
    {
        m_actor->m_state = Actor::State::Movement;
    }

    if (Input::isSet(Input::Jump))
    {
        auto ledge = m_actor->findLedge();

        if (ledge.pos != g_vec0)
        {
            enterClimb(ledge);
        }
    }

    if (!m_focused && Input::isSet(Input::Focus))
    {
        pickClosestFocus();
        m_focused = true;
        enterIdle();
    }

    m_actor->m_physCont.highFriction();
    m_actor->m_pos = m_actor->m_physCont.getPosition();
}

void PlayerController::updateFocusedIdle(float deltaTime)
{
    if (!Input::isSet(Input::Focus))
    {
        m_focused = false;
        enterIdle();
    }

    if (Input::isSet(Input::Up) ||
        Input::isSet(Input::Down) ||
        Input::isSet(Input::Left) ||
        Input::isSet(Input::Right))
    {
        m_actor->m_state = Actor::State::Movement;
    }

    g_renderer.m_debugRenderer.addLine(m_focus->getPosition(), m_focus->getPosition() + vec3f(0.f,10.f,0.f), vec3f(1.f,0.f,0.f));

    if (Input::isSet(Input::Action))
        enterPunch();
    if (Input::isSet(Input::Block))
        enterBlock();
    if (Input::isSet(Input::Jump))
        enterDodge();

    m_actor->m_dir = glm::normalize(m_focus->getPosition() - m_actor->getPosition());
    m_actor->m_yaw = atan2(m_actor->m_dir.x, m_actor->m_dir.z);

    m_actor->m_physCont.highFriction();
    m_actor->m_pos = m_actor->m_physCont.getPosition();
}

void PlayerController::updateIdle(float deltaTime)
{
    if (!m_focused)
        updateFreeIdle(deltaTime);
    else
        updateFocusedIdle(deltaTime);
}

void PlayerController::updateFreeMovement(float deltaTime)
{
    if (Input::isSet(Input::Sprint))
        m_actor->m_speed = 20;
    else
        m_actor->m_speed = 12;

    bool hasMoved = false;

    vec3f direction;

    if (Input::isSet(Input::Up))
    {
        direction += m_camera.getForwardVec();
        hasMoved = true;
    }
    else if (Input::isSet(Input::Down))
    {
        direction += -m_camera.getForwardVec();
        hasMoved = true;
    }

    if (Input::isSet(Input::Left))
    {
        direction += -m_camera.getSidewayVec();
        hasMoved = true;
    }
    else if (Input::isSet(Input::Right))
    {
        direction += m_camera.getSidewayVec();
        hasMoved = true;
    }

    direction = glm::normalize(direction);

    if (glm::dot(direction, m_actor->m_dir) > -0.5f)
        m_actor->m_dir += direction * 0.25;
    else
        m_actor->m_dir = direction;

    m_actor->m_dir = glm::normalize(m_actor->m_dir);
    m_actor->setAnimation("Guy_run.x");

    if (!hasMoved)
        enterIdle();
    else
        moveAlongGround(direction);

    if (!m_focused && Input::isSet(Input::Focus))
    {
        pickClosestFocus();
        m_focused = true;
    }

    if (Input::isSet(Input::Jump))
    {
        enterJump(18.f);
    }

    m_actor->m_pos = m_actor->m_physCont.getPosition();
}

void PlayerController::updateFocusedMovement(float deltaTime)
{
    if (!Input::isSet(Input::Focus))
    {
        m_focused = false;
    }

    g_renderer.m_debugRenderer.addLine(m_focus->getPosition(), m_focus->getPosition() + vec3f(0.f,10.f,0.f), vec3f(1.f,0.f,0.f));

    bool hasMoved = false;

    vec3f direction;

    if (Input::isSet(Input::Up))
    {
        direction += m_camera.getForwardVec();
        hasMoved = true;
    }
    else if (Input::isSet(Input::Down))
    {
        direction += -m_camera.getForwardVec();
        hasMoved = true;
    }

    if (Input::isSet(Input::Left))
    {
        direction += -m_camera.getSidewayVec();
        hasMoved = true;
    }
    else if (Input::isSet(Input::Right))
    {
        direction += m_camera.getSidewayVec();
        hasMoved = true;
    }

    direction = glm::normalize(direction);

    m_actor->m_dir = glm::normalize(m_focus->getPosition() - m_actor->getPosition());
    m_actor->m_yaw = atan2(m_actor->m_dir.x, m_actor->m_dir.z);

    m_actor->setAnimation("Guy_run.x");

    if (!hasMoved)
        enterIdle();
    else
        moveAlongGround(direction);

    m_actor->m_pos = m_actor->m_physCont.getPosition();
}

void PlayerController::updateMovement(float deltaTime)
{
    if (!m_focused)
        updateFreeMovement(deltaTime);
    else
        updateFocusedMovement(deltaTime);
}

void PlayerController::updatePunch(float deltaTime)
{
    Base::updatePunch(deltaTime);

    if (m_controls[Controls::Block])
        enterBlock();
}

void PlayerController::updateBlock(float deltaTime)
{
    Base::updateBlock(deltaTime);

    if (m_controls[Controls::Action])
        enterPunch();
}
