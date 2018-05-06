#include "Actor.hpp"
#include "World.hpp"
#include "Messages/Message.hpp"
#include "Resources/Resources.hpp"
#include "Render/Renderer3D.hpp"
#include "Core/Config.hpp"
#include <SDL2/SDL.h>

static u32 gNextActorId = 0;

Actor::Actor()
{
    _id = gNextActorId++;
}

void Actor::initAnimMesh()
{
    _animMesh.init(*gResources.getAnimatedMesh("humalea.dae").get());
    _animMesh.material = gResources.getMaterial("Munkey.mat").get();
    gRenderer3D.registerAnimMesh(&_animMesh);

    setAnimation("Guy_idle.dae");
}

void Actor::init(World* world)
{
    _world = world;

    initAnimMesh();

    _decoration.init(mat4(1.f));
}

void Actor::processEvents(const SDL_Event& event)
{
}

void Actor::sendMessage(const Message& message)
{
}

void Actor::preUpdate(seconds deltaTime)
{
}

void Actor::postUpdate(seconds deltaTime)
{
    if (_pos.x > 30)
        _pos.x = 30;
    if (_pos.x < -30)
        _pos.x = -30;

    if (_pos.z > 30)
        _pos.z = 30;
    if (_pos.z < -30)
        _pos.z = -30;

    _animMesh.update(deltaTime);

    _transform = math::translate(_pos) * math::rotate(_yaw, math::vecY);
    _animMesh.m_transform = _transform;

    _attachTrs[AttachSpot::RightHand] = _transform * (*_animMesh.getJointTransform("Rig_RightHandAttach"));
    _attachTrs[AttachSpot::LeftHand] = _transform * (*_animMesh.getJointTransform("Rig_LeftHandAttach"));

    _decoration.setTransform(_transform * (*_animMesh.getJointTransform("Rig_RightHandAttach")));
}

void Actor::setAnimation(const std::string& name,
    seconds time, bool loop, bool* trigger, seconds triggerTime
    )
{
    auto anim = gResources.getAnimation(name).get();
    _animMesh.setAnimation(anim, time, loop, trigger, triggerTime);
}

bool Actor::isDead() const
{
    return _health <= 0;
}
