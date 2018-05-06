#ifndef HUMAN_CONTROLLER_HPP
#define HUMAN_CONTROLLER_HPP
#include "ActorController.hpp"
#include "Actor.hpp"

class HumanController : public ActorController
{
    public:
        using Base = ActorController;

        HumanController() = default;
        HumanController(Actor* character);

        void processEvents(const SDL_Event& event) override final;
        void preUpdate(float deltaTime)            override final;
        void postUpdate(float deltaTime)           override final;

        void sendEvent(const WorldEvent& worldEvt) override final;

    private:
        enum class MindState
        {
            Idle,
            Routine,
            Combat,
            Follow,
            GetToPoint
        };

        enum class Strafe
        {   
            None,
            Left,
            Right
        };
        
        void setMindState(MindState state);

        void updateIdle(float deltaTime)           override final;

        void enterMovement(Strafe strafe);
        void updateMovement(float deltaTime)       override final;
        void updateMovementFollow(float deltaTime);
        void updateMovementCombat(float deltaTime);

        void onDamage(const WorldEvent& worldEvt) override final;
        void onDeath(const WorldEvent& worldEvt);

    private:
        enum class NextCombatAction : uint32
        {
            Idle = 0,
            Attack,
            StrafeLeft,
            StrafeRight,
            Block,
            
            NumActions
        };

        MindState           m_mindState;
        Strafe              m_strafe;
        NextCombatAction    m_nextAction;

    friend class Actor;
};

#endif
