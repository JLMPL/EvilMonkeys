#pragma once
#include "Actor/ActorState.hpp"
#include "Core/Types.hpp"

struct Message;
struct Actor;
class World;
class Camera;

class PlayerController
{
    public:
        PlayerController() = default;

        void init(Actor* actor, World* world, Camera* camera);
        void update(seconds deltaTime);

        void sendMessage(const Message& message);

    private:
        void initStates();
        void setState(ActorState::Type state, i32 flags = 0);

    private:
        World* m_world = nullptr;
        Actor* m_actor = nullptr;
        Camera* m_camera = nullptr;

        ActorState* m_state = nullptr;
        ActorState::Ptr m_states[ActorState::Type::Count];
};
