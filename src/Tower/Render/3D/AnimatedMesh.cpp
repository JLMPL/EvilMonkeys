#include "AnimatedMesh.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

void AnimatedMesh::loadFromFile(const Path& path)
{
    using namespace rapidxml;

    std::ifstream file(path);
    std::string content;

    if (!file.good())
    {
        Log::log(Log::Type::Error, "Could not load Mesh %s\n", path.c_str());
        exit(EXIT_FAILURE);
    }
    else
    {
        while (!file.eof())
        {
            std::string line;
            std::getline(file, line);
            content += line + '\n';
        }
    }

    xml_document<> doc;
    doc.parse<0>(&content[0]);

    xml_node<>* root = doc.first_node();
    xml_node<>* firstNode = root->first_node("library_visual_scenes")->
        first_node("visual_scene")->
        first_node("node")->
        first_node("node");

    loadSkeletonJoints(firstNode);
    std::vector<VertexWeightData> vwd;
    loadWeights(root, vwd);
    loadMeshes(root, vwd);
    calcTangents(entry);
}

i8 AnimatedMesh::loadSkeletonJoints(rapidxml::xml_node<>* node)
{
    using namespace rapidxml;

    i8 jointIndex = skeleton.joints.size();

    Joint joint;
    joint.name = node->first_attribute("id")->value();
    joint.nodeTransform = math::translate(vec3(10));
    joint.children.resize(g_maxChildrenPerBone);

    for (auto& i : joint.children)
        i = -1;

    skeleton.joints.push_back(joint);

    xml_node<>* iter = node->first_node("node");

    u32 i = 0;
    while (iter)
    {
        i8 childJointIndex = loadSkeletonJoints(iter);
        skeleton.joints[jointIndex].children[i] = childJointIndex;

        iter = iter->next_sibling();
        i++;
    }

    return jointIndex;
}

void AnimatedMesh::loadMeshes(rapidxml::xml_node<>* root, std::vector<VertexWeightData>& vwd)
{
    using namespace rapidxml;

    xml_node<>* geometry = root->first_node("library_geometries")->first_node("geometry");
    xml_node<>* xmlPositions = geometry->first_node("mesh")->first_node("source")->first_node("float_array");
    xml_node<>* xmlNormals = geometry->first_node("mesh")->first_node("source")->next_sibling()->first_node("float_array");
    xml_node<>* xmlCoords = geometry->first_node("mesh")->first_node("source")->next_sibling()->next_sibling()->first_node("float_array");
    xml_node<>* xmlColors = geometry->first_node("mesh")->first_node("source")->next_sibling()->next_sibling()->next_sibling()->first_node("float_array");
    xml_node<>* xmlIndices = geometry->first_node("mesh")->first_node("triangles");

    u32 numPositions = std::atoi(xmlPositions->first_attribute("count")->value()) /3;
    u32 numNormals = std::atoi(xmlNormals->first_attribute("count")->value()) /3;
    u32 numCoords = std::atoi(xmlCoords->first_attribute("count")->value()) /2;
    u32 numColors = std::atoi(xmlColors->first_attribute("count")->value()) /3;
    u32 numIndices = std::atoi(xmlIndices->first_attribute("count")->value()) * 3;

    std::string posData = xmlPositions->value();
    std::string normalData = xmlNormals->value();
    std::string coordData = xmlCoords->value();
    std::string colorData = xmlColors->value();
    std::string indexData = xmlIndices->first_node("p")->value();

    std::vector<vec3> verts;
    std::vector<vec3> normals;
    std::vector<vec2> coords;
    std::vector<vec4> colors;

    std::vector<vec4u> indices;

    std::stringstream ss(posData);

    for (u32 i = 0; i < numPositions; i++)
    {
        float x,y,z;
        ss >> x >> y >> z;
        verts.push_back({x, y, z});
    }

    ss.clear();
    ss.str(normalData);
    for (u32 i = 0; i < numNormals; i++)
    {
        float x,y,z;
        ss >> x >> y >> z;
        normals.push_back({x, y, z});
    }

    ss.clear();
    ss.str(coordData);
    for (u32 i = 0; i < numCoords; i++)
    {
        float x,y;
        ss >> x >> y;
        coords.push_back({x, 1-y});
    }

    ss.clear();
    ss.str(colorData);
    for (u32 i = 0; i < numColors; i++)
    {
        float r,g,b;
        ss >> r >> g >> b;
        colors.push_back({r, g, b, 1});
    }

    ss.clear();
    ss.str(indexData);
    for (u32 i = 0; i < numIndices; i++)
    {
        u32 vert, normal, coord, color;
        ss >> vert >> normal >> coord >> color;

        vec4u ind;
        ind[0] = vert;
        ind[1] = normal;
        ind[2] = coord;
        ind[3] = color;

        indices.push_back(ind);
    }

    std::vector<vec3> sortedVerts;
    std::vector<VertexWeightData> sortedWeights;
    std::vector<vec3> sortedNormals;
    std::vector<vec2> sortedCoords;
    std::vector<vec4> sortedColors;

    for (u32 i = 0; i < indices.size(); i++)
    {
        sortedVerts.push_back(verts[indices[i][0]]);
        sortedWeights.push_back(vwd[indices[i][0]]);
        sortedNormals.push_back(normals[indices[i][1]]);
        sortedCoords.push_back(coords[indices[i][2]]);
        sortedColors.push_back(colors[indices[i][3]]);
    }

    std::vector<AnimatedMeshEntry::Triangle> sortedIndices;

    for (u32 i = 0; i < sortedVerts.size(); i += 3)
    {
        AnimatedMeshEntry::Triangle tris;
        tris.verts[0] = i;
        tris.verts[1] = i+1;
        tris.verts[2] = i+2;

        sortedIndices.push_back(tris);
    }

    u32 numVerts = sortedVerts.size();

    entry.vertices.resize(numVerts);
    entry.normals.resize(numVerts);
    entry.colors.resize(numVerts);
    entry.coords.resize(numVerts);
    entry.weightsData.resize(numVerts);

    entry.tangents.resize(numVerts);
    entry.bitangents.resize(numVerts);
    entry.indices.resize(sortedIndices.size());

    for (u32 i = 0; i < numVerts; i++)
    {
        entry.vertices[i] = sortedVerts[i];
        entry.weightsData[i] = sortedWeights[i];
        entry.normals[i] = sortedNormals[i];
        entry.colors[i] = sortedColors[i];
        entry.coords[i] = sortedCoords[i];
    }

    for (u32 i = 0; i < sortedIndices.size(); i++)
    {
        entry.indices[i] = sortedIndices[i];
    }

    xml_node<>* theShit = root->first_node("library_controllers")->
        first_node("controller")->
        first_node("skin");

    xml_node<>* names = theShit->first_node("source");
    xml_node<>* binds = names->next_sibling();

    std::string namesData = names->first_node("Name_array")->value();
    std::string bindsData = binds->first_node("float_array")->value();

    std::stringstream sn(namesData);
    std::stringstream sb(bindsData);

    for(uint32_t i = 0; i < skeleton.joints.size(); i++)
    {
        std::string name;
        mat4 matrix;

        sn >> name;

        sb >> matrix[0][0] >> matrix[1][0] >> matrix[2][0] >> matrix[3][0];
        sb >> matrix[0][1] >> matrix[1][1] >> matrix[2][1] >> matrix[3][1];
        sb >> matrix[0][2] >> matrix[1][2] >> matrix[2][2] >> matrix[3][2];
        sb >> matrix[0][3] >> matrix[1][3] >> matrix[2][3] >> matrix[3][3];

        Joint* joint = skeleton.findJoint("Rig_" + name);
        joint->offsetMatrix = matrix;
    }
}

void AnimatedMesh::loadWeights(rapidxml::xml_node<>* root, std::vector<VertexWeightData>& vwd)
{
    using namespace rapidxml;

    xml_node<>* jointNames = root->first_node("library_controllers")->
        first_node("controller")->
        first_node("skin")->
        first_node("source")->
        first_node("Name_array");

    xml_node<>* assocs = root->first_node("library_controllers")->
        first_node("controller")->
        first_node("skin")->
        first_node("vertex_weights");

    xml_node<>* xmlWeights = assocs->previous_sibling()->previous_sibling();

    std::string namesData = jointNames->value();
    std::string weightsData = xmlWeights->first_node("float_array")->value();
    std::string numsData = assocs->first_node("vcount")->value();
    std::string jwsData = assocs->first_node("v")->value();

    u32 numNames = std::atoi(jointNames->first_attribute("count")->value());
    u32 numWeights = std::atoi(xmlWeights->first_node("float_array")->first_attribute("count")->value());
    u32 numNums = std::count(numsData.begin(), numsData.end(), ' ') + 1;
    u32 numJws = std::count(jwsData.begin(), jwsData.end(), ' ') + 1;

    struct JointWeight
    {
        u32 joint, weight;
    };

    std::vector<std::string> names;
    std::vector<float> weights;
    std::vector<u32> nums;
    std::vector<JointWeight> jws;

    std::stringstream snnn(namesData);

    for (u32 i = 0; i < numNames; i++)
    {
        std::string name;
        snnn >> name;
        names.push_back("Rig_" + name);
    }

    std::stringstream sw(weightsData);

    for (u32 i = 0; i < numWeights; i++)
    {
        float weight;
        sw >> weight;
        weights.push_back(weight);
    }

    std::stringstream sn(numsData);

    for (u32 i = 0; i < numNums; i++)
    {
        u32 num;
        sn >> num;
        nums.push_back(num);
    }

    std::stringstream sj(jwsData);

    for (u32 i = 0; i < numJws; i++)
    {
        JointWeight jw;
        sj >> jw.joint >> jw.weight;
        jws.push_back(jw);
    }

    u32 prevs = 0;

    for (u32 i = 0; i < numNums; i++)
    {
        VertexWeightData data;
        for (u32 j = prevs; j < prevs + nums[i]; j++)
        {
            data.addData(skeleton.findJointIndex(names[jws[j].joint]), weights[jws[j].weight]);
        }
        vwd.push_back(data);
        prevs += nums[i];
    }
}

void AnimatedMesh::calcTangents(AnimatedMeshEntry& entry)
{
    std::vector<vec3> tan1(entry.vertices.size());
    std::vector<vec3> tan2(entry.vertices.size());

    for (auto i = 0; i < entry.indices.size(); i++)
    {
        u32 i1 = entry.indices[i].verts[0];
        u32 i2 = entry.indices[i].verts[1];
        u32 i3 = entry.indices[i].verts[2];

        const vec3& v1 = entry.vertices[i1];
        const vec3& v2 = entry.vertices[i2];
        const vec3& v3 = entry.vertices[i3];

        const vec2& w1 = entry.coords[i1];
        const vec2& w2 = entry.coords[i2];
        const vec2& w3 = entry.coords[i3];

        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;

        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;

        float r = 1.f / (s1 * t2 - s2 * t1);

        vec3 sdir(
            (t2 * x1 - t1 * x2) * r,
            (t2 * y1 - t1 * y2) * r,
            (t2 * z1 - t1 * z2) * r
        );
        vec3 tdir(
            (s1 * x2 - s2 * x1) * r,
            (s1 * y2 - s2 * y1) * r,
            (s1 * z2 - s2 * z1) * r
        );

        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;

        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }

    for (auto i = 0; i < entry.vertices.size(); i++)
    {
        const vec3& n = entry.normals[i];
        const vec3& t = tan1[i];

        entry.tangents[i] = math::normalize(t - n * math::dot(n, t));
        float w = (math::dot(math::cross(n, t), tan2[i]) < 0.f) ? -1.f : 1.f;

        entry.bitangents[i] = math::cross(n, entry.tangents[i]) * w;
    }
}
