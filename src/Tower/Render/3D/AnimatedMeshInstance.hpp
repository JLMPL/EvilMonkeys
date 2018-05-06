#pragma once
#include "AnimatedMesh.hpp"
#include "Animation.hpp"
#include "../Shader.hpp"

struct Material;

class AnimatedMeshInstance
{
    public:
        AnimatedMeshInstance() = default;

        void init(const AnimatedMesh& mesh);
        void update(seconds deltaTime);
        void render(const Shader& shader) const;

        void setAnimation(const Animation* anim, seconds time, bool loop = true, bool* trigger = nullptr, seconds triggerTime = seconds(0));

        mat4* getJointTransform(const std::string& name);

        bool isFinished() const;
        seconds getDuration() const;

    private:
        void genBufferObjects(const AnimatedMeshEntry& entry);

        void animate(const Animation& animation, Joint& bone, const mat4& parentTransform);

        vec3 lerpPosition(const NodeAnimation& nodeAnimation, seconds animationTime);
        quat lerpRotation(const NodeAnimation& nodeAnimation, seconds animationTime);

    public:
        mat4 m_transform;
        const Material* material;

    private:
        VertexArray         m_vao;
        ArrayBuffer         m_buffers[NumBuffers];
        u32                 m_numVertices = 0u;

        Skeleton            m_skeleton;
        mat4                m_correctionMatrix;

        seconds             m_animationTime = seconds(0);
        const Animation*    m_currAnim = nullptr;
        const Animation*    m_lastAnim = nullptr;

        Animation           m_transition;
        bool                m_isTransition = false;

        bool*               m_trigger = nullptr;
        seconds             m_triggerTime = seconds(0);
        seconds             m_timer = seconds(0);

        bool                m_loop = true;
        bool                m_finished = false;
};
