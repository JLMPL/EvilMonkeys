#pragma once
#include "Entity.hpp"
#include "Decoration.hpp"
#include "Render/3D/AnimatedMeshInstance.hpp"

class World;

struct LedgeData
{
    vec3 pos;
    vec3 normal;
    float height = 0.f;
};

struct Actor
{
    using Ptr = std::unique_ptr<Actor>;

    Actor();

    void init(World* world);
    void initAnimMesh();

    void processEvents(const SDL_Event& event);
    void preUpdate(seconds deltaTime);
    void postUpdate(seconds deltaTime);

    void sendMessage(const Message& message);

    void setAnimation(const std::string& anim,
        seconds time = seconds(0.2),
        bool loop = true,
        bool* trigger = nullptr,
        seconds triggerTime = seconds(0.0)
    );

    bool isDead() const;

    enum AttachSpot
    {
        RightHand,
        LeftHand,
        Belt,
        Number
    };

    World*                  _world;
    u32                     _id;

    AnimatedMeshInstance    _animMesh;

    mat4                    _transform;
    vec3                    _pos;
    f32                     _yaw = 0.f;
    f32                     _speed = 10.f;
    vec3                    _dir = {0,0,1};

    i32                     _health = 100;

    mat4                    _attachTrs[AttachSpot::Number];
    Decoration              _decoration;

    const Actor*            _focus = nullptr;
};
