#ifndef ACTOR_CONTROLLER_HPP
#define ACTOR_CONTROLLER_HPP
#include "WorldEvent.hpp"
#include "Core/Math3d.hpp"
#include <memory>

union SDL_Event;

class Actor;
class Entity;
class World;

constexpr float g_lowHeightThreshold = 2.5f;
constexpr float g_midHeightThreshold = 7.f;

class ActorController
{
    public:
        using Ptr = std::unique_ptr<ActorController>;

        ActorController() = default;
        ActorController(Actor* character);

        virtual void processEvents(const SDL_Event& event) = 0;
        virtual void preUpdate(float deltaTime) = 0;
        virtual void postUpdate(float deltaTime) = 0;

        virtual void sendEvent(const WorldEvent& worldEvt) = 0;

        uint32 getActorsId();

    protected:
        void switchStates(float deltaTime);

        void keepOnGround();
        void moveAlongGround(const vec3f& dir);

        virtual void enterIdle();
        virtual void updateIdle(float deltaTime);

        virtual void enterMovement();
        virtual void updateMovement(float deltaTime);

        virtual void enterJump(float speed);
        virtual void updateJump(float deltaTime);

        virtual void enterClimb(LedgeData ledge);
        virtual void updateClimb(float deltaTime);

        virtual void enterPunch();
        virtual void updatePunch(float deltaTime);

        virtual void enterDeath();
        virtual void updateDeath(float deltaTime);

        virtual void enterGetHit();
        virtual void updateGetHit(float deltaTime);

        virtual void enterDodge();
        virtual void updateDodge(float deltaTime);

        virtual void enterBlock();
        virtual void updateBlock(float deltaTime);

        //EVENTS
        virtual void onDamage(const WorldEvent& worldEvt);

    protected:
        Actor*        m_actor = nullptr;
        World*        m_world = nullptr;

        Entity*       m_focus = nullptr;
        bool          m_focused = false;

        LedgeData     m_ledge;
        int           m_ledgeHeightType = 0;
        bool          m_punchNow = false;

        float         m_timer = 0.f;

    friend class Actor;
};

#endif

//THE BORROWER IS THE SLAVE TO THE LENDER
