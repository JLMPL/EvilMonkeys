#pragma once
#include "Core/Types.hpp"
#include "Core/Math3d.hpp"
#include <memory>

class World;

union SDL_Event;
struct Message;

class Entity
{
    public:
        using Ptr = std::unique_ptr<Entity>;

        template <typename T>
        T* as()
        {
            return dynamic_cast<T*>(this);
        }

        Entity();
        virtual ~Entity() = default;

        virtual void processEvents(const SDL_Event& event) = 0;
        virtual void preUpdate(seconds deltaTime) = 0;
        virtual void postUpdate(seconds deltaTime) = 0;

        virtual void sendMessage(const Message& message) = 0;

        void move(const vec3& offset);

        void setId(u32 id);
        u32 getId() const;

        virtual void setPosition(const vec3& pos);
        const vec3& getPosition() const;

        void destroy();
        bool isDestroyed() const;

    protected:
        World*    m_world;
        u32    m_id;

        vec3     m_pos;
        mat4     m_transform;

        bool      m_destroyed = false;
        // bool      m_active = true;
};
