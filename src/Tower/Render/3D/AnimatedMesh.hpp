#pragma once
#include "Skeleton.hpp"
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"
#include "Core/Types.hpp"
#include <rapidxml/rapidxml.hpp>
#include <vector>
#include <string>
#include <memory>

constexpr u32 g_maxChildrenPerBone = 8u;

struct VertexWeightData
{
    i32 joints[4] = {0};
    float weights[4] = {0.f};

    void addData(const i32& id, float weight)
    {
        for (auto i = 0; i < 4; i++)
        {
            if (weights[i] == 0.f)
            {
                joints[i] = id;
                weights[i] = weight;
                return;
            }
        }
    }
};

enum Buffer
{
    Vertex = 0,
    Normal,
    Color,
    TexCoord,
    Joints,
    Weights,
    Tangents,
    BiTangents,
    Index,

    NumBuffers
};

struct AnimatedMeshEntry
{
    struct Triangle
    {
        GLuint verts[3];
    };

    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec4> colors;
    std::vector<vec2> coords;
    std::vector<vec3> tangents;
    std::vector<vec3> bitangents;

    std::vector<VertexWeightData> weightsData;

    std::vector<Triangle> indices;
};

struct AnimatedMesh
{
    using Ptr = std::shared_ptr<AnimatedMesh>;

    AnimatedMeshEntry entry;
    Skeleton skeleton;

    void loadFromFile(const Path& path);
    i8 loadSkeletonJoints(rapidxml::xml_node<>* node);
    void loadMeshes(rapidxml::xml_node<>* node, std::vector<VertexWeightData>& vwd);
    void loadWeights(rapidxml::xml_node<>* root, std::vector<VertexWeightData>& vwd);
    void calcTangents(AnimatedMeshEntry& entry);
};
