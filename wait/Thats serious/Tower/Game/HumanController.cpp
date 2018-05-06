#include "HumanController.hpp"
#include "Messages/MessageBus.hpp"
#include "Actor.hpp"
#include "World.hpp"
#include "Actor/States.hpp"
#include "Core/Config.hpp"
#include "Core/Random.hpp"
#include <array>

using Chain = std::array<HumanController::CombatAction, 2>;

static constexpr std::array<Chain, 18> CombatActions =
{{
    //DEFFENSIVE
    {{HumanController::CombatAction::Block, HumanController::CombatAction::StrafeRight}},
    {{HumanController::CombatAction::Block, HumanController::CombatAction::StrafeLeft}},
    {{HumanController::CombatAction::StrafeLeft, HumanController::CombatAction::Stab}},
    {{HumanController::CombatAction::StrafeRight, HumanController::CombatAction::Stab}},

    {{HumanController::CombatAction::StrafeLeft, HumanController::CombatAction::Attack}},
    {{HumanController::CombatAction::StrafeRight, HumanController::CombatAction::Attack}},
    {{HumanController::CombatAction::Block, HumanController::CombatAction::Stab}},
    {{HumanController::CombatAction::Block, HumanController::CombatAction::Attack}},

    //OFFENSIVE
    {{HumanController::CombatAction::StrafeRight, HumanController::CombatAction::Attack}},
    {{HumanController::CombatAction::StrafeLeft, HumanController::CombatAction::Attack}},
    {{HumanController::CombatAction::Stab, HumanController::CombatAction::Attack}},
    {{HumanController::CombatAction::Attack, HumanController::CombatAction::Stab}},

    {{HumanController::CombatAction::Attack, HumanController::CombatAction::Block}},
    {{HumanController::CombatAction::Stab, HumanController::CombatAction::Block}},

    {{HumanController::CombatAction::StrafeLeft, HumanController::CombatAction::Stab}},
    {{HumanController::CombatAction::StrafeRight, HumanController::CombatAction::Stab}},
    {{HumanController::CombatAction::Stab, HumanController::CombatAction::StrafeLeft}},
    {{HumanController::CombatAction::Stab, HumanController::CombatAction::StrafeRight}}
}};

void HumanController::init(Actor* actor, World* world)
{
    m_world = world;
    m_actor = actor;

    initStates();
    setState(ActorState::Type::Idle, 0);

    m_actor->_focus = m_world->getEntityById(0);
}

void HumanController::initStates()
{
    m_states[ActorState::Type::Idle]     = std::unique_ptr<HumanIdle>(new HumanIdle());
    m_states[ActorState::Type::Movement] = std::unique_ptr<HumanMovement>(new HumanMovement());
    m_states[ActorState::Type::Attack]   = std::unique_ptr<ActorAttack>(new ActorAttack());
    m_states[ActorState::Type::Block]    = std::unique_ptr<ActorBlock>(new ActorBlock());
    m_states[ActorState::Type::Dead]     = std::unique_ptr<ActorDead>(new ActorDead());
    m_states[ActorState::Type::Pain]     = std::unique_ptr<ActorPain>(new ActorPain());
    m_states[ActorState::Type::Dodge]    = std::unique_ptr<ActorDodge>(new ActorDodge());
}

void HumanController::sendMessage(const Message& message)
{
    if (message.type == Message::Type::KeyPressed)
    {
        if (message.keyboard.key == Config::Controls::Cheat)
        {
            m_actor->_health -= 100;
        }
    }
    else if (message.recieverEntity == m_actor->_id)
    {
        if (message.type == Message::Type::ActorSetState)
        {
            i32 state = message.state;

            setState(ActorState::Type(state), message.stateFlags);
            m_chainStep++;
        }
        else if (message.type == Message::Type::ActorAboutToAttack)
        {
            if (Random::inRangei(1,100) > 40)
                pickDefensiveChain();
        }
        else if (message.type == Message::Type::ActorDamage)
        {
            if (m_state->isVulnerable())
            {
                m_actor->_health -= message.ivalue;

                if (m_actor->isDead())
                    setState(ActorState::Type::Dead, 0);
                else
                    setState(ActorState::Type::Pain, 0);

                Message message(Message::Type::InterfaceSetEnemyHealth);
                message.ivalue = m_actor->_health;
                MessageBus::queueMessage(message);
            }
        }
    }

    m_state->sendMessage(message);
}

void HumanController::update(seconds deltaTime)
{
    m_timer += deltaTime;

    if (!m_actor->isDead())
    {
        m_actor->_dir = math::normalize(m_actor->_focus->_pos - m_actor->_pos);
        m_actor->_yaw = atan2(m_actor->_dir.x, m_actor->_dir.z);
    }

    m_state->update(deltaTime);

    if (m_currentChain != -1 && !m_actor->isDead())
    {
        executeChainStep(CombatActions[m_currentChain][m_chainStep]);

        if (m_chainStep == 1)
        {
            m_currentChain = -1;
            m_chainStep = 0;
        }
    }
    else if (!m_actor->isDead())
    {
        if (m_timer >= seconds(5))
        {
            pickOffensiveChain();
            m_timer = seconds(0);
        }
    }
}

void HumanController::executeChainStep(CombatAction action)
{
    switch(action)
    {
        case CombatAction::Attack:
            setState(ActorState::Type::Attack, 0);
            break;
        case CombatAction::Stab:
            setState(ActorState::Type::Attack, 1);
            break;
        case CombatAction::StrafeLeft:
            setState(ActorState::Type::Dodge, 0);
            break;
        case CombatAction::StrafeRight:
            setState(ActorState::Type::Dodge, 1);
            break;
        case CombatAction::Block:
            setState(ActorState::Type::Block, 0);
            break;
    }
}

void HumanController::pickDefensiveChain()
{
    m_currentChain = Random::inRangei(0,7);
    m_chainStep = 0;
}

void HumanController::pickOffensiveChain()
{
    m_currentChain = Random::inRangei(8,17);
    m_chainStep = 0;
}

void HumanController::setState(ActorState::Type state, u32 flags)
{
    ActorState* prevState = m_state;

    m_state = m_states[state].get();

    if (prevState != m_state)
        m_state->enter(m_actor, flags);
}
