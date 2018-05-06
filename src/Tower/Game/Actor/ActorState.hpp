#ifndef ACTOR_STATE_HPP
#define ACTOR_STATE_HPP
#include "Core/Math3d.hpp"
#include <memory>

#define BEGIN_ACTOR_STATE(x)                                   \
class x : public ActorState                                    \
{                                                              \
    public:                                                    \
        void enter(Actor* actor, i32 flags) override final;\
        void sendMessage(const Message& event) override final; \
        void update(seconds deltaTime) override final;         \
                                                               \
        bool isVulnerable() override final;                    \
    private:                                                   \

#define END_ACTOR_STATE() \
};

struct Actor;
struct Message;

class ActorState
{
    public:
        using Ptr = std::unique_ptr<ActorState>;

        enum Type : u32
        {
            Idle = 0,
            Movement,
            Attack,
            Dead,
            Pain,
            Dodge,
            Block,

            Count
        };

        virtual void enter(Actor* actor, i32 flags) = 0;
        virtual void sendMessage(const Message& event) = 0;
        virtual void update(seconds deltaTime) = 0;

        virtual bool isVulnerable() = 0;

    protected:
        void setState(Type state, u32 flags = 0);

    protected:
        Actor* m_actor = nullptr;
};

#endif
