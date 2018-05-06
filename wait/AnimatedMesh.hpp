#ifndef AnimatedMesh_hpp
#define AnimatedMesh_hpp
#include "Animation.hpp"
#include "Skeleton.hpp"
#include "Core/Types.hpp"
#include "Render/OpenGL.hpp"
#include <assimp/scene.h>
#include <vector>
#include <string>
#include <map>

class Ragdoll;

struct Triangle
{
    GLuint verts[3];
};

struct skinned_vertex_t
{
    glm::vec3 pos;
    int32 bone_id = -1;
    float weight = 0.f;
};

struct AnimatedMeshEntry
{
    // std::vector<glm::vec3> vertices;
    std::vector<skinned_vertex_t> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec2> coords;

    std::vector<glm::vec3> transformedVertices;
    std::vector<glm::vec3> transformedNormals;

    std::vector<Triangle> indices;

    GLuint vao;

    GLuint vbo;
    GLuint nbo;
    GLuint cbo;
    GLuint tbo;

    GLuint ibo;
};

class AnimatedMesh
{
    public:
        using Ptr = std::shared_ptr<AnimatedMesh>;

        AnimatedMesh() = default;
        ~AnimatedMesh();

        void loadFromFile(const std::string& path);

        void update(float deltaTime, Ragdoll* ragdoll = nullptr);
        void render() const;

        void setAnimation(const Animation* anim, float time);

        void genBufferObjects();

    private:
        int8 addNodesToSkeleton(aiNode* node);
        void addMeshesAndBones(const aiScene* scene);

        void readNodeHierarchy(const Animation& animation, Bone& bone, const glm::mat4& parentTransform);
        void transformVertices();
        void updateBuffer();

        glm::vec3 findInterpolatedPosition(const NodeAnimation& nodeAnimation, float animationTime);
        glm::quat findInterpolatedRotation(const NodeAnimation& nodeAnimation, float animationTime);

    private:
        AnimatedMeshEntry    m_entry;
        Skeleton             m_skeleton;
        glm::mat4            m_globalInverseTransform;

        float                m_animationTime = 0.f;
        const Animation*     m_currAnim = nullptr;
        const Animation*     m_lastAnim = nullptr;

        Animation            m_transition;
        bool                 m_isTransition = false;
};

#endif