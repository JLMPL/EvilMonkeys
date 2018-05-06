#pragma once
#include "Core/Math3d.hpp"

struct Message;

class Camera
{
    public:
        Camera() = default;

        void init();
        void update();

        void sendMessage(const Message& message);

        mat4 getView() const;

        void setYaw(const vec3& vec);

        void setEye(const vec3& eye);
        void setCenter(const vec3& center);

        vec3 getPos() const;
        vec3 getTarget() const;

        vec3 getForwardVec() const;
        vec3 getSidewayVec() const;

    private:
        void mouseLook();
        void movementLerp();
        void calcMats();
        void checkCollisions();

    private:
        mat4     m_transform;

        float    m_pitch = 0.f;
        float    m_yaw = 0.f;

        vec3     m_eye;
        vec3     m_center;

        vec3     m_targetEye;
        vec3     m_targetCenter;

        vec3     m_offset;

        vec3     m_dir;
        mat4     m_view;

        vec2     m_relative;
};
