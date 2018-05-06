#include "Animation.hpp"
#include "Core/Convert.hpp"
#include "Core/Types.hpp"
#include "Core/Log.hpp"
#include <rapidxml/rapidxml.hpp>
#include <fstream>
#include <sstream>

void Animation::loadCollada(const Path& path)
{
    using namespace rapidxml;

    std::ifstream file(path.c_str());
    std::string content;

    if (!file.good())
    {
        Log::log(Log::Type::Error, "Could not load Animation %s\n", path.c_str());
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
    xml_node<>* firstAnimation = root->first_node("library_animations")->first_node("animation");

    xml_node<>* iter = firstAnimation;

    while (iter)
    {
        NodeAnimation nodeAnimation;

        std::string name = iter->first_attribute("id")->value();
        name = name.substr(0, name.size() - 12);

        nodeAnimation.name = name;

        xml_node<>* times = iter->first_node("source");
        xml_node<>* xforms = iter->first_node("source")->next_sibling();

        std::string timesData = times->first_node("float_array")->value();
        std::string xformsData = xforms->first_node("float_array")->value();

        u32 numKeys = std::atoi(times->first_node("float_array")->first_attribute("count")->value());

        std::stringstream st(timesData);
        std::stringstream sx(xformsData);

        for (u32 i = 0; i < numKeys; i++)
        {
            float time;
            mat4 matrix;

            st >> time;

            sx >> matrix[0][0] >> matrix[1][0] >> matrix[2][0] >> matrix[3][0];
            sx >> matrix[0][1] >> matrix[1][1] >> matrix[2][1] >> matrix[3][1];
            sx >> matrix[0][2] >> matrix[1][2] >> matrix[2][2] >> matrix[3][2];
            sx >> matrix[0][3] >> matrix[1][3] >> matrix[2][3] >> matrix[3][3];

            // printf("%f\n", time);
            // printf(
            //     "%f %f %f %f\n"
            //     "%f %f %f %f\n"
            //     "%f %f %f %f\n"
            //     "%f %f %f %f\n",

            //     matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
            //     matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
            //     matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
            //     matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]
            // );

            NodeAnimationKey<vec3> posKey;
            NodeAnimationKey<quat> rotKey;

            posKey.time = time;
            rotKey.time = time;

            posKey.value = vec3(matrix[3]);
            rotKey.value = math::quat_cast(matrix);

            nodeAnimation.positionKeys.push_back(posKey);
            nodeAnimation.rotationKeys.push_back(rotKey);
        }

        nodeAnimations.push_back(nodeAnimation);
        iter = iter->next_sibling("animation");
    }

    duration = seconds(nodeAnimations[0].positionKeys.back().time);
}

void Animation::loadFromFile(const Path& path)
{
    loadCollada(path);
}

const NodeAnimation* Animation::findNodeAnim(const std::string& nodeName) const
{
    for (auto& i : nodeAnimations)
        if (i.name == nodeName)
            return &i;

    return nullptr;
}
