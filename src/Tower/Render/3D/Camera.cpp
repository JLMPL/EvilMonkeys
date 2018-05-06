#include "Camera.hpp"
#include "Messages/Message.hpp"
#include "Core/Convert.hpp"

constexpr f32 g_lerpPerc = 0.2f;

void Camera::init()
{
    m_offset = vec3(0.f,5.f,-12.f);
}

void Camera::sendMessage(const Message& message)
{
    if (message.type == Message::Type::MouseMoved)
    {
        m_relative.x = message.mouse.relx;
        m_relative.y = message.mouse.rely;
    }
    if (message.type == Message::Type::CameraMove)
    {
        m_targetCenter.x = message.point.x;
        m_targetCenter.y = message.point.y;
        m_targetCenter.z = message.point.z;
    }
}

void Camera::mouseLook()
{
    m_yaw -= float(m_relative.x) / 500.f;
    m_pitch += float(m_relative.y) / 500.f;

    if (m_pitch > HALF_PI - 0.2f)
        m_pitch = HALF_PI - 0.2f;
    if (m_pitch < -HALF_PI + 0.2f)
        m_pitch = -HALF_PI + 0.2f;

    float fullRad = M_PI*2;

    if (m_yaw < 0)
        m_yaw += fullRad;
    if (m_yaw > fullRad)
        m_yaw -= fullRad;
}

void Camera::movementLerp()
{
    m_transform =
        math::translate(m_targetCenter) *
        math::rotate(m_yaw, math::vecY) *
        math::rotate(m_pitch, math::vecX);

    m_targetEye = vec3(m_transform * vec4(m_offset, 1));

    vec3 aim = m_targetCenter - m_center;

    if (math::length(aim) > 0.01)
    {
        m_center += aim * g_lerpPerc;
    }

    aim = m_targetEye - m_eye;

    if (math::length(aim) > 0.01)
    {
        m_eye += aim * g_lerpPerc;
    }
}

void Camera::checkCollisions()
{
    if (m_eye.y < 0)
        m_eye.y = 0;

    if (m_eye.x > 30.f)
        m_eye.x = 30.f;

    if (m_eye.x < -30.f)
        m_eye.x = -30.f;

    if (m_eye.z > 30.f)
        m_eye.z = 30.f;

    if (m_eye.z < -30.f)
        m_eye.z = -30.f;
}

void Camera::calcMats()
{
    m_dir = math::normalize(m_center - m_eye);

    m_view = math::lookAt(m_eye, m_center, vec3(0,1,0));
}

void Camera::update()
{
    // mouseLook();
    // movementLerp();
    checkCollisions();
    calcMats();

    m_relative = vec2(0);
}

void Camera::setYaw(const vec3& vec)
{
    m_yaw = atan2(vec.x, vec.z);
}

void Camera::setEye(const vec3& eye)
{
    m_eye = eye;
}

void Camera::setCenter(const vec3& center)
{
    m_center = center;
}

mat4 Camera::getView() const
{
    return m_view;
}

vec3 Camera::getPos() const
{
    return m_eye;
}

vec3 Camera::getTarget() const
{
    return m_center;
}

vec3 Camera::getForwardVec() const
{
    return math::normalize(vec3(m_dir.x, 0.f, m_dir.z));
}

vec3 Camera::getSidewayVec() const
{
    return math::normalize(math::cross(m_dir, vec3(0.f,1.f,0.f)));
}
