#include "AnimatedMesh.hpp"
#include "Core/Convert.hpp"
#include "Core/Types.hpp"
#include "Physics/Ragdoll.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

AnimatedMesh::~AnimatedMesh()
{
    if (m_entry.vao) GLCALL(glDeleteVertexArrays(1, &m_entry.vao));
    if (m_entry.vbo) GLCALL(glDeleteBuffers(1, &m_entry.vbo));
    if (m_entry.nbo) GLCALL(glDeleteBuffers(1, &m_entry.nbo));
    if (m_entry.cbo) GLCALL(glDeleteBuffers(1, &m_entry.cbo));
    if (m_entry.tbo) GLCALL(glDeleteBuffers(1, &m_entry.tbo));
    if (m_entry.ibo) GLCALL(glDeleteBuffers(1, &m_entry.ibo));
}

int8_t AnimatedMesh::addNodesToSkeleton(aiNode* node)
{
    int8_t boneID = m_skeleton.bones.size();
    {
        Bone bone;
        bone.name = node->mName.C_Str();
        bone.nodeTransform = aiToGlm(node->mTransformation);
        bone.Children.resize(node->mNumChildren);

        for(uint32_t i = 0; i < bone.Children.size(); i++)
            bone.Children[i] = -1;

        m_skeleton.bones.push_back(bone);
    }

    for(uint32_t i = 0; i < node->mNumChildren; i++)
    {
        int8_t childBoneID = addNodesToSkeleton(node->mChildren[i]);
        m_skeleton.bones[boneID].Children[i] = childBoneID;
    }

    return boneID;
}

void AnimatedMesh::addMeshesAndBones(const aiScene* scene)
{
    for(uint32_t i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];

        AnimatedMeshEntry entry;

        entry.vertices.resize(mesh->mNumVertices);
        entry.normals.resize(mesh->mNumVertices);
        entry.coords.resize(mesh->mNumVertices);
        entry.colors.resize(mesh->mNumVertices);

        entry.transformedVertices.resize(mesh->mNumVertices);
        entry.transformedNormals.resize(mesh->mNumVertices);

        entry.indices.resize(mesh->mNumFaces);

        for(uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            entry.vertices[i].x = mesh->mVertices[i].x;
            entry.vertices[i].y = mesh->mVertices[i].y;
            entry.vertices[i].z = mesh->mVertices[i].z;

            entry.normals[i].x = mesh->mNormals[i].x;
            entry.normals[i].y = mesh->mNormals[i].y;
            entry.normals[i].z = mesh->mNormals[i].z;

            entry.coords[i].x = mesh->mTextureCoords[0][i].x;
            entry.coords[i].y = mesh->mTextureCoords[0][i].y;

            if (mesh->GetNumColorChannels() > 0)
            {
                entry.colors[i].x = mesh->mColors[0][i].r;
                entry.colors[i].y = mesh->mColors[0][i].g;
                entry.colors[i].z = mesh->mColors[0][i].b;
                entry.colors[i].w = mesh->mColors[0][i].a;
            }
            else
                entry.colors[i] = {1,1,1,1};
        }

        entry.transformedVertices = entry.vertices;
        entry.transformedNormals = entry.normals;

        for(uint32_t i = 0; i < mesh->mNumBones; i++)
        {
            aiBone* abon = mesh->mBones[i];
            Bone& bone = *m_skeleton.findBone(abon->mName.C_Str());
            bone.offsetMatrix = aiToGlm(abon->mOffsetMatrix);
            bone.Weights.resize(abon->mNumWeights);

            for(uint32_t i = 0; i < bone.Weights.size(); i++)
            {
                bone.Weights[i].vertexID = abon->mWeights[i].mVertexId;
                bone.Weights[i].weight = abon->mWeights[i].mWeight;
            }
        }

        for(uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            entry.indices[i].verts[0] = face.mIndices[0];
            entry.indices[i].verts[1] = face.mIndices[1];
            entry.indices[i].verts[2] = face.mIndices[2];
        }

        m_entry = entry;
    }
}

void AnimatedMesh::loadFromFile(const std::string& path)
{
    Assimp::Importer Importer;
    const aiScene* scene = Importer.ReadFile(path.c_str(), aiProcess_LimitBoneWeights | aiProcess_FlipUVs | aiProcess_MakeLeftHanded);

    m_globalInverseTransform = aiToGlm(scene->mRootNode->mTransformation);
    m_globalInverseTransform *= glm::rotate(glm::mat4(1.f), 90.0_rad, glm::vec3(1,0,0));
    m_globalInverseTransform = glm::inverse(m_globalInverseTransform);

    addNodesToSkeleton(scene->mRootNode);
    addMeshesAndBones(scene);
}

void AnimatedMesh::update(float deltaTime, Ragdoll* ragdoll)
{
    if (!ragdoll)
    {
        if (!m_isTransition)
        {
            m_animationTime = fmodf(m_animationTime + deltaTime * m_currAnim->ticksPerSecond, m_currAnim->duration);
            readNodeHierarchy(*m_currAnim, m_skeleton.bones[0], glm::mat4(1.f));
        }
        else
        {
            m_animationTime += deltaTime * m_transition.ticksPerSecond;

            if (m_animationTime >= m_transition.duration)
            {
                m_isTransition = false;
                m_animationTime = 0;
            }
            else
                readNodeHierarchy(m_transition, m_skeleton.bones[0], glm::mat4(1.f));
        }
        transformVertices();
        updateBuffer();
    }
    else
    {
        const std::vector<std::string> bonz = {
            "Armature_chest",
            "Armature_head",
            "Armature_upperArm_L",
            "Armature_upperArm_R",
            "Armature_foreArm_L",
            "Armature_foreArm_R",
            "Armature_ass",
            "Armature_thigh_L",
            "Armature_thigh_R",
            "Armature_calf_L",
            "Armature_calf_R"
        };

        for (auto i = 0; i < bonz.size(); i++)
        {
            auto bony = m_skeleton.findBone(bonz[i]);

            glm::mat4 scle = glm::scale(glm::vec3(0.f,0.f,-1.f));

            bony->finalTransformation = 
                // m_globalInverseTransform *
                ragdoll->getLimb(bonz[i]).motionState->getTransform() *
                // bony->offsetMatrix *
                glm::translate(glm::vec3(0.f,1.5f,0.f)) *
                glm::rotate(glm::mat4(1.f), 90.0_rad, glm::vec3(1.f,0.f,0.f));
        }

        transformVertices();
        updateBuffer();
    }
}

void AnimatedMesh::render() const
{
    GLCALL(glBindVertexArray(m_entry.vao));
    GLCALL(glDrawElements(GL_TRIANGLES, m_entry.vertices.size(), GL_UNSIGNED_INT, 0));
    GLCALL(glBindVertexArray(0));
}

void AnimatedMesh::genBufferObjects()
{
    GLCALL(glGenVertexArrays(1, &m_entry.vao));
    GLCALL(glBindVertexArray(m_entry.vao));

    GLCALL(glGenBuffers(1, &m_entry.vbo));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_entry.vbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m_entry.transformedVertices.size(), &m_entry.transformedVertices[0], GL_DYNAMIC_DRAW));

    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));

    GLCALL(glGenBuffers(1, &m_entry.nbo));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_entry.nbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m_entry.transformedNormals.size(), &m_entry.transformedNormals[0], GL_DYNAMIC_DRAW));

    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0));

    GLCALL(glGenBuffers(1, &m_entry.cbo));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_entry.cbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * m_entry.colors.size(), &m_entry.colors[0], GL_STATIC_DRAW));

    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0));

    GLCALL(glGenBuffers(1, &m_entry.tbo));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_entry.tbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * m_entry.coords.size(), &m_entry.coords[0], GL_STATIC_DRAW));

    GLCALL(glEnableVertexAttribArray(3));
    GLCALL(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0));

    GLCALL(glGenBuffers(1, &m_entry.ibo));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_entry.ibo));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * m_entry.indices.size(), &m_entry.indices[0], GL_STATIC_DRAW));

    GLCALL(glBindVertexArray(0));
}

void AnimatedMesh::updateBuffer()
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_entry.vbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m_entry.transformedVertices.size(), &m_entry.transformedVertices[0], GL_DYNAMIC_DRAW));

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_entry.nbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m_entry.transformedNormals.size(), &m_entry.transformedNormals[0], GL_DYNAMIC_DRAW));

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void AnimatedMesh::setAnimation(const Animation* anim, float time)
{
    if (m_currAnim != anim)
    {
        m_lastAnim = m_currAnim;
        m_currAnim = anim;

        if (m_lastAnim != nullptr)
        {
            m_transition.duration = time;
            m_transition.ticksPerSecond = 1;

            m_transition.nodeAnimations.clear();

            for (auto i = 0; i < m_lastAnim->nodeAnimations.size(); i++)
            {
                NodeAnimation na;
                na.name = m_lastAnim->nodeAnimations[i].name;

                const NodeAnimation* newNodeAnim = m_lastAnim->findNodeAnim(na.name);
                glm::vec3 translation = findInterpolatedPosition(*newNodeAnim, m_animationTime);
                glm::quat rotation = findInterpolatedRotation(*newNodeAnim, m_animationTime);

                NodeAnimationKey<glm::vec3> lastpos = {translation, 0};
                NodeAnimationKey<glm::quat> lastrot = {rotation, 0};

                NodeAnimationKey<glm::vec3> newpos = {m_currAnim->nodeAnimations[i].positionKeys[0].value, time};
                NodeAnimationKey<glm::quat> newrot = {m_currAnim->nodeAnimations[i].rotationKeys[0].value, time};
                
                na.positionKeys.push_back(lastpos);
                na.rotationKeys.push_back(lastrot);

                na.positionKeys.push_back(newpos);
                na.rotationKeys.push_back(newrot);

                m_transition.nodeAnimations.push_back(na);
            }

            m_animationTime = 0;
            m_isTransition = true;
        }
    }
}

void AnimatedMesh::readNodeHierarchy(const Animation& animation, Bone& joint, const glm::mat4& parentTransform)
{
    std::string nodeName = joint.name;
    glm::mat4 nodeTransformation = joint.nodeTransform;
    const NodeAnimation* newNodeAnim = animation.findNodeAnim(nodeName);

    if (newNodeAnim)
    {
        glm::vec3 position = findInterpolatedPosition(*newNodeAnim, m_animationTime);
        glm::quat rotation = findInterpolatedRotation(*newNodeAnim, m_animationTime);

        glm::mat4 translation = glm::translate(position);
        glm::mat4 orientation = glm::toMat4(rotation);

        nodeTransformation = translation * orientation;
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransformation;
    // joint.attachmentTransform = m_globalInverseTransform * globalTransformation;

    auto boneIndex = m_skeleton.findBoneIndex(nodeName);
    if (boneIndex != -1)
    {
        Bone& bone = m_skeleton.bones[boneIndex];
        bone.finalTransformation = m_globalInverseTransform * globalTransformation * bone.offsetMatrix;
    }

    for (auto i = 0; i < joint.Children.size(); i++)
        readNodeHierarchy(animation, m_skeleton.bones[joint.Children[i]], globalTransformation);
}

void AnimatedMesh::transformVertices()
{
    for (auto i = 0; i < m_entry.transformedVertices.size(); i++)
    {
        m_entry.transformedVertices[i] = {0,0,0};
        m_entry.transformedNormals[i] = {0,0,0};
    }

    for (const auto& bone : m_skeleton.bones)
    {
        glm::mat4  transform = bone.finalTransformation;
        glm::mat3x3 rotation = glm::mat3x3(transform);

        for (auto& weight : bone.Weights)
        {
            glm::vec3 inVertex = m_entry.vertices[weight.vertexID];
            glm::vec3& outVertex = m_entry.transformedVertices[weight.vertexID];
            outVertex += glm::vec3((transform * glm::vec4(inVertex, 1)) * weight.weight);

            glm::vec3 inNormal = m_entry.normals[weight.vertexID];
            glm::vec3& outNormal = m_entry.transformedNormals[weight.vertexID];
            outNormal += (rotation * inNormal) * weight.weight;
        }
    }

    for (auto& i : m_entry.normals)
        i = glm::normalize(i);
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

glm::vec3 AnimatedMesh::findInterpolatedPosition(const NodeAnimation& nodeAnimation, float animationTime)
{
    auto& keys = nodeAnimation.positionKeys;

    if (keys.size() == 1)
        return keys[0].value;

    uint32_t currPosIndex = findCurrentFrameIndex(keys, animationTime);
    uint32_t nextPosIndex = (currPosIndex + 1);

    float difference = keys[nextPosIndex].time - keys[currPosIndex].time;
    float factor = (animationTime - keys[currPosIndex].time) / difference;

    return glm::lerp(keys[currPosIndex].value, keys[nextPosIndex].value, factor);
}

glm::quat AnimatedMesh::findInterpolatedRotation(const NodeAnimation& nodeAnimation, float animationTime)
{
    auto& keys = nodeAnimation.rotationKeys;

    if (keys.size() == 1)
        return keys[0].value;

    uint32_t currPosIndex = findCurrentFrameIndex(keys, animationTime);
    uint32_t nextPosIndex = (currPosIndex + 1);

    float difference = keys[nextPosIndex].time - keys[currPosIndex].time;
    float factor = (animationTime - keys[currPosIndex].time) / difference;

    return glm::slerp(keys[currPosIndex].value, keys[nextPosIndex].value, factor);
}
