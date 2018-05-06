#ifndef PLAYER_CONTROLLER_HPP
#define PLAYER_CONTROLLER_HPP
#include "ActorController.hpp"
#include "Actor.hpp"

class PlayerController : public ActorController
{
    public:
        using Base = ActorController;

        PlayerController() = default;
        PlayerController(Actor* character);

        void processEvents(const SDL_Event& event) override final;
        void preUpdate(float deltaTime)            override final;
        void postUpdate(float deltaTime)           override final;

        void sendEvent(const WorldEvent& worldEvt) override final;

    private:
        void pickClosestFocus();
        void updateCamera();

        void enterIdle()                           override final;
        void updateFreeIdle(float deltaTime);
        void updateFocusedIdle(float deltaTime);
        void updateIdle(float deltaTime)           override final;

        void updateFreeMovement(float deltaTime);
        void updateFocusedMovement(float deltaTime);
        void updateMovement(float deltaTime)       override final;

        void updatePunch(float deltaTime)          override final;
        void updateBlock(float deltaTime)          override final;

    private:
        enum Controls
        {
            Up = 0,
            Down,
            Left,
            Right,
            Jump,
            Action,
            Focus,
            Sprint,
            Block,

            NumControls
        };

        bool      m_controls[NumControls] = {false};

        Camera    m_camera;

        mat4f     m_cameraTransform;
        vec3f     m_cameraPos;
        vec3f     m_cameraLerpPos[2];
        float     m_cameraLerpPerc = 0.f;
        float     m_cameraPitch = 0.f;
        float     m_cameraYaw = 0.f;

        int32     m_relativeMouseX = 0;
        int32     m_relativeMouseY = 0;

        bool m_turningAround = false;
};

#endif
