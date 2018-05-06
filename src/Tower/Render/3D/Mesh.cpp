#include "Mesh.hpp"
#include "Core/Log.hpp"
#include "Core/Convert.hpp"
#include "Core/Random.hpp"
#include "Resources/Resources.hpp"
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#define SSCANF sscanf_s
#else
#define SSCANF sscanf
#endif

void Mesh::loadObj(const Path& path)
{
    struct Face
    {
        u32 verts[3];
        u32 coords[3];
        u32 normals[3];
    };

    std::vector<vec3> verts;
    std::vector<vec3> norms;
    std::vector<vec2> coords;
    std::vector<Face> indices;

    std::ifstream file(path.c_str());

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

            if (line[0] == 'v' && line[1] == ' ')
            {
                vec3 pos;
                SSCANF(line.c_str(), "v %f %f %f", &pos.x, &pos.y, &pos.z);
                verts.push_back(pos);
            }
            else if (line[0] == 'v' && line[1] == 'n')
            {
                vec3 normal;
                SSCANF(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                norms.push_back(normal);
            }
            else if (line[0] == 'v' && line[1] == 't')
            {
                vec2 coord;
                SSCANF(line.c_str(), "vt %f %f", &coord.x, &coord.y);
                coords.push_back(coord);
            }
            else if (line[0] == 'f' && line[1] == ' ')
            {
                Face face;
                SSCANF(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &face.verts[0], &face.coords[0], &face.normals[0],
                    &face.verts[1], &face.coords[1], &face.normals[1],
                    &face.verts[2], &face.coords[2], &face.normals[2]
                );
                indices.push_back(face);
            }
        }
    }

    std::vector<vec3> sortedVerts;
    std::vector<vec3> sortedNormals;
    std::vector<vec4> sortedColors;
    std::vector<vec2> sortedCoords;

    for (u32 i = 0; i < indices.size(); i++)
    {
        sortedVerts.push_back(verts[indices[i].verts[0]-1]);
        sortedVerts.push_back(verts[indices[i].verts[1]-1]);
        sortedVerts.push_back(verts[indices[i].verts[2]-1]);

        sortedNormals.push_back(norms[indices[i].normals[0]-1]);
        sortedNormals.push_back(norms[indices[i].normals[1]-1]);
        sortedNormals.push_back(norms[indices[i].normals[2]-1]);

        sortedColors.push_back({1,1,1,1});
        sortedColors.push_back({1,1,1,1});
        sortedColors.push_back({1,1,1,1});

        sortedCoords.push_back(coords[indices[i].coords[0]-1]);
        sortedCoords.push_back(coords[indices[i].coords[1]-1]);
        sortedCoords.push_back(coords[indices[i].coords[2]-1]);
    }

    std::vector<MeshTriangle> indicess;

    for (u32 i = 0; i < sortedVerts.size(); i += 3)
    {
        MeshTriangle tris;
        tris.inds[0] = i;
        tris.inds[1] = i+1;
        tris.inds[2] = i+2;

        indicess.push_back(tris);
    }

    m_entry.create(sortedVerts, sortedNormals, sortedColors, sortedCoords, indicess);
    m_entry.setupBuffers();
}

void Mesh::loadCollada(const Path& path)
{
    using namespace rapidxml;

    std::ifstream file(path.c_str()); //fancy
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
        coords.push_back({x, y});
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
    std::vector<vec3> sortedNormals;
    std::vector<vec2> sortedCoords;
    std::vector<vec4> sortedColors;

    for (u32 i = 0; i < indices.size(); i++)
    {
        sortedVerts.push_back(verts[indices[i][0]]);
        sortedNormals.push_back(normals[indices[i][1]]);
        sortedCoords.push_back(coords[indices[i][2]]);
        sortedColors.push_back(colors[indices[i][3]]);
    }

    std::vector<MeshTriangle> sortedIndices;

    for (u32 i = 0; i < sortedVerts.size(); i += 3)
    {
        MeshTriangle tris;
        tris.inds[0] = i;
        tris.inds[1] = i+1;
        tris.inds[2] = i+2;

        sortedIndices.push_back(tris);
    }

    m_entry.create(sortedVerts, sortedNormals, sortedColors, sortedCoords, sortedIndices);
    m_entry.setupBuffers();
}

void Mesh::loadFromFile(const Path& path)
{
    std::size_t dotPos = path.find_last_of('.');
    std::string extension = path.substr(dotPos + 1, 3);

    // printf("%s\n", extension.c_str());

    if (extension == "obj")
        loadObj(path);
    else if (extension == "dae")
        loadCollada(path);
    else
    {
        Log::log(Log::Type::Error, "Could not load mesh %s unsupported file format!\n", path.c_str());
        exit(EXIT_FAILURE);
    }
}

void Mesh::render(u32 index) const
{
    // m_entries[index].render();
    m_entry.render();
}

/* no bullet
PhysicsData Mesh::addCollisionMesh(PhysicsWorld* physics) //UR NOT THE OWNER
{
    auto mesh = physics->addTriangleMesh();

    for (auto& entry : m_entries)
    for (auto i = 0; i < entry.m_numTriangles; i++)
    {
        vec3 a = entry.m_vertices[entry.m_tris[i].inds[0]];
        vec3 b = entry.m_vertices[entry.m_tris[i].inds[1]];
        vec3 c = entry.m_vertices[entry.m_tris[i].inds[2]];

        mesh->addTriangle(glmToBt(a), glmToBt(b), glmToBt(c));
    }

    mat4 transform(1.f);
    physics->finalizeTriangleMesh(vec3(transform[3]), math::quat_cast(transform));
}
*/

float Mesh::getCullRadius() const
{
    return m_cullRadius;
}
