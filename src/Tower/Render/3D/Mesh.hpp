#pragma once
#include "MeshEntry.hpp"
#include <memory>

class Mesh
{
    public:
        using Ptr = std::shared_ptr<Mesh>;

        Mesh() = default;
        ~Mesh() = default;

        void loadFromFile(const Path& path);
        void render(u32 index) const;

        // PhysicsData addCollisionMesh(PhysicsWorld* physics); no bullet
        float getCullRadius() const;

    private:
        void loadObj(const Path& path);
        void loadCollada(const Path& path);

    private:
        MeshEntry m_entry;

        float m_cullRadius = 1000.f; // COMPUTE LATER

    friend class SceneRenderer;
};
