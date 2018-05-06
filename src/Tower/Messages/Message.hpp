#pragma once
#include "Core/Types.hpp"

struct Message
{
    enum Type : u32
    {
        KeyPressed = 0,
        MouseMoved,
        MousePressed,
        CameraMove,

        EngineChangeResolution,

        GameStateSet,
        GameStateExit,

        SoundPlay,

        InterfaceSetHealth,
        InterfaceSetEnemyHealth,

        ActorMoveForward,
        ActorTurnLeft,
        ActorSetState,
        ActorDamage,
        ActorAboutToAttack,

        ParticlesSpawn
    };

    u32 type;

    struct Keyboard
    {
        i32 key;
    };

    struct Mouse
    {
        i32 relx;
        i32 rely;
        bool left;
        bool right;
    };

    struct Point
    {
        f32 x,y,z;
    };

    struct Particles
    {
        u32 num;
        Point pos;
        Point dir;
        Point color;
        f32 velocity;
        f32 weight;
        f32 life;
    };

    union
    {
        Keyboard keyboard;
        Mouse mouse;
        Point point;
        Particles particles;

        i32 ivalue;
        i32 width;
        i32 height;

        struct
        {
            i32 state;
            u32 stateFlags;
        };
    };

    i32 senderEntity = -1;
    i32 recieverEntity = -1;

    Message() = default;
    Message(Type tp) : type(tp) {}
};
