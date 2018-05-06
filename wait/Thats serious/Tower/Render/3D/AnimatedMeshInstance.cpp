#include "AnimatedMeshInstance.hpp"
#include "Core/Convert.hpp"
#include "Core/Types.hpp"

void AnimatedMeshInstance::init(const AnimatedMesh& mesh)
{
    m_skeleton = mesh.skeleton;
    m_numVertices = mesh.entry.vertices.size();

    m_correctionMatrix = math::rotate(-90.0_rad, vec3(1,0,0));

    genBufferObjects(mesh.entry);
}

void AnimatedMeshInstance::update(seconds deltaTime)
{
    using namespace chli;

    if (!m_isTransition)
    {
        m_animationTime += seconds(deltaTime);
        m_timer += deltaTime;

        if (m_trigger)
        {
            if (m_timer > m_triggerTime)
            {
                *m_trigger = true;
                m_trigger = nullptr;
            }
        }

        if (!m_loop)
            m_finished = false;
        else
            m_finished = true;

        if (m_animationTime >= m_currAnim->duration)
        {
            if (m_loop)
            {
                m_animationTime = 0s;
                m_timer = 0s;
            }
            else
            {
                m_animationTime = m_currAnim->duration - seconds(0.0001);
                m_finished = true;
            }
        }

        animate(*m_currAnim, m_skeleton.joints[0], mat4(1.f));
    }
    else
    {
        m_finished = false;

        m_animationTime += seconds(deltaTime);

        if (m_animationTime >= m_transition.duration)
        {
            m_isTransition = false;
            m_animationTime = 0s;
        }
        else
            animate(m_transition, m_skeleton.joints[0], mat4(1.f));
    }
}

void AnimatedMeshInstance::render(const Shader& shader) const
{
    for (auto i = 0; i < m_skeleton.joints.size(); i++)
    {
        std::string index = "bones[" + std::to_string(i) + "]";

        GL(glUniformMatrix4fv(shader.getUniformLocation(index.c_str()),
            1, GL_FALSE,
            &m_skeleton.joints[i].finalTransformation[0][0]
        ));
    }

    m_vao.bind();
    GL(glDrawElements(GL_TRIANGLES, m_numVertices, GL_UNSIGNED_INT, 0));
    m_vao.unbind();
}

void AnimatedMeshInstance::genBufferObjects(const AnimatedMeshEntry& entry)
{
    for (auto i = 0; i < Buffer::NumBuffers-1; i++)
    {
        m_buffers[i].init(GL_ARRAY_BUFFER);
    }

    m_vao.init();
    m_vao.bind();

    //vertices
    m_buffers[Buffer::Vertex].bind();
    m_buffers[Buffer::Vertex].setData(sizeof(float) * 3 * entry.vertices.size(), &entry.vertices[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //normals
    m_buffers[Buffer::Normal].bind();
    m_buffers[Buffer::Normal].setData(sizeof(float) * 3 * entry.normals.size(), &entry.normals[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //colors
    m_buffers[Buffer::Color].bind();
    m_buffers[Buffer::Color].setData(sizeof(float) * 4 * entry.colors.size(), &entry.colors[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

    //texcoords
    m_buffers[Buffer::TexCoord].bind();
    m_buffers[Buffer::TexCoord].setData(sizeof(float) * 2 * entry.coords.size(), &entry.coords[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);

    //bonez
    m_buffers[Buffer::Joints].bind();
    m_buffers[Buffer::Joints].setData(sizeof(VertexWeightData) * entry.weightsData.size(), &entry.weightsData[0], GL_STATIC_DRAW);

    GL(glEnableVertexAttribArray(4));
    GL(glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexWeightData), 0));

    //weightes
    m_buffers[Buffer::Weights].bind();
    m_buffers[Buffer::Weights].setData(sizeof(VertexWeightData) * entry.weightsData.size(), &entry.weightsData[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexWeightData), (const GLvoid*)(offsetof(VertexWeightData, weights)));

    //tangents
    m_buffers[Buffer::Tangents].bind();
    m_buffers[Buffer::Tangents].setData(sizeof(float) * 3 * entry.tangents.size(), &entry.tangents[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //bitangents
    m_buffers[Buffer::BiTangents].bind();
    m_buffers[Buffer::BiTangents].setData(sizeof(float) * 3 * entry.bitangents.size(), &entry.bitangents[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //indices
    m_buffers[Buffer::Index].init(GL_ELEMENT_ARRAY_BUFFER);
    m_buffers[Buffer::Index].bind();
    m_buffers[Buffer::Index].setData(sizeof(GLuint) * 3 * entry.indices.size(), &entry.indices[0], GL_STATIC_DRAW);

    m_vao.unbind();
}

void AnimatedMeshInstance::setAnimation(const Animation* anim, seconds time, bool loop, bool* trigger, seconds triggerTime)
{
    using namespace chli;

    if (m_currAnim != anim)
    {
        m_lastAnim = m_currAnim;
        m_currAnim = anim;

        m_trigger = trigger;
        m_triggerTime = triggerTime;
        m_timer = 0s;

        m_loop = loop;

        if (m_lastAnim != nullptr)
        {
            m_transition.duration = time;

            m_transition.nodeAnimations.clear();

            for (auto i = 0; i < m_lastAnim->nodeAnimations.size(); i++)
            {
                NodeAnimation na;
                na.name = m_lastAnim->nodeAnimations[i].name;

                const NodeAnimation* newNodeAnim = m_lastAnim->findNodeAnim(na.name);
                vec3 translation = lerpPosition(*newNodeAnim, m_animationTime);
                quat rotation = lerpRotation(*newNodeAnim, m_animationTime);

                NodeAnimationKey<vec3> lastpos = {translation, 0};
                NodeAnimationKey<quat> lastrot = {rotation, 0};

                NodeAnimationKey<vec3> newpos = {m_currAnim->nodeAnimations[i].positionKeys[0].value, time.count()};
                NodeAnimationKey<quat> newrot = {m_currAnim->nodeAnimations[i].rotationKeys[0].value, time.count()};

                na.positionKeys.push_back(lastpos);
                na.rotationKeys.push_back(lastrot);

                na.positionKeys.push_back(newpos);
                na.rotationKeys.push_back(newrot);

                m_transition.nodeAnimations.push_back(na);
            }

            m_animationTime = 0s;
            m_isTransition = true;
        }
    }
}

void AnimatedMeshInstance::animate(const Animation& animation, Joint& joint, const mat4& parentTransform)
{
    mat4 nodeTransformation;
    const NodeAnimation* nodeAnim = animation.findNodeAnim(joint.name);

    if (nodeAnim)
    {
        vec3 position = lerpPosition(*nodeAnim, m_animationTime);
        quat rotation = lerpRotation(*nodeAnim, m_animationTime);

        mat4 translation = math::translate(position);
        mat4 orientation = math::toMat4(rotation);

        nodeTransformation = translation * orientation;
    }

    mat4 globalTransformation = parentTransform * nodeTransformation;
    joint.attachment = m_correctionMatrix * globalTransformation;

    auto jointIndex = m_skeleton.findJointIndex(joint.name);
    if (jointIndex != -1)
    {
        Joint& joint = m_skeleton.joints[jointIndex];
        joint.finalTransformation = m_correctionMatrix * globalTransformation * joint.offsetMatrix;
    }

    for (auto i = 0; i < g_maxChildrenPerBone; i++)
    {
        if (joint.children[i] != -1)
            animate(animation, m_skeleton.joints[joint.children[i]], globalTransformation);
    }
}

template<typename T>
static uint32_t findCurrentFrameIndex(const T& keys, float animationTime)
{
    for (uint32_t i = 0; i < keys.size() - 1; i++)
    {
        if (animationTime < keys[i + 1].time)
            return i;
    }
    return -1;
}

vec3 AnimatedMeshInstance::lerpPosition(const NodeAnimation& nodeAnimation, seconds animationTime)
{
    auto& keys = nodeAnimation.positionKeys;

    if (keys.size() == 1)
        return keys[0].value;

    uint32_t currPosIndex = findCurrentFrameIndex(keys, animationTime.count());
    uint32_t nextPosIndex = (currPosIndex + 1);

    float difference = keys[nextPosIndex].time - keys[currPosIndex].time;
    float factor = (animationTime.count() - keys[currPosIndex].time) / difference;

    return math::lerp(keys[currPosIndex].value, keys[nextPosIndex].value, factor);
}

quat AnimatedMeshInstance::lerpRotation(const NodeAnimation& nodeAnimation, seconds animationTime)
{
    auto& keys = nodeAnimation.rotationKeys;

    if (keys.size() == 1)
        return keys[0].value;

    uint32_t currPosIndex = findCurrentFrameIndex(keys, animationTime.count());
    uint32_t nextPosIndex = (currPosIndex + 1);

    float difference = keys[nextPosIndex].time - keys[currPosIndex].time;
    float factor = (animationTime.count() - keys[currPosIndex].time) / difference;

    return math::slerp(keys[currPosIndex].value, keys[nextPosIndex].value, factor);
}

mat4* AnimatedMeshInstance::getJointTransform(const std::string& name)
{
    auto* joint = m_skeleton.findJoint(name);
    return &joint->attachment;
}

bool AnimatedMeshInstance::isFinished() const
{
    return m_finished;
}

seconds AnimatedMeshInstance::getDuration() const
{
    if (m_currAnim)
    {
        return m_currAnim->duration;
    }
    return seconds(1);
}
