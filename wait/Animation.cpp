#include "Animation.hpp"
#include "Core/Convert.hpp"
#include "Core/Types.hpp"
#include "Core/Log.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

bool Animation::loadFromFile(const std::string& path)
{
    Assimp::Importer Importer;
    const aiScene* scene = Importer.ReadFile(path.c_str(), aiProcess_LimitBoneWeights | aiProcess_FlipUVs | aiProcess_MakeLeftHanded);

    if(scene->mNumAnimations > 0)
    {
        aiAnimation* animation = scene->mAnimations[0];

        for(auto i = 0; i < animation->mNumChannels; i++)
        {
            aiNodeAnim* channel = animation->mChannels[i];

            NodeAnimation nodeAnimation;
            nodeAnimation.name = channel->mNodeName.C_Str();

            for(auto i = 0; i < channel->mNumPositionKeys; i++)
            {
                NodeAnimationKey<glm::vec3> posKey;
                posKey.time = float(channel->mPositionKeys[i].mTime);
                posKey.value = aiToGlm(channel->mPositionKeys[i].mValue);
                nodeAnimation.positionKeys.push_back(posKey);
            }

            for(auto i = 0; i < channel->mNumRotationKeys; i++)
            {
                NodeAnimationKey<glm::quat> quatKey;
                quatKey.time = float(channel->mRotationKeys[i].mTime);
                quatKey.value = aiToGlm(channel->mRotationKeys[i].mValue);
                nodeAnimation.rotationKeys.push_back(quatKey);
            }

            nodeAnimations.push_back(nodeAnimation);
        }

        ticksPerSecond = float(scene->mAnimations[0]->mTicksPerSecond);
        duration = float(scene->mAnimations[0]->mDuration);
    }
    else
        Log::log(Log::Type::Error, "Could not find any animations in file \"%s\"", path.c_str());
}

const NodeAnimation* Animation::findNodeAnim(const std::string& nodeName) const
{
    for (auto& i : nodeAnimations)
        if (i.name == nodeName)
            return &i;

    return nullptr;
}
