#pragma once
#include "Shader.hpp"
#include "ScreenQuad.hpp"
#include "3D/Skybox.hpp"
#include "PostProcess/Bloom.hpp"
#include "Core/OrderlessArray.hpp"
#include "Core/Math3d.hpp"
#include <vector>
#include <array>

class Camera;

struct Light;
struct MeshInstance;
struct FoliageInstance;
class AnimatedMeshInstance;
class GrassInstance;

class Renderer3D
{
    public:
        struct Settings
        {
            f32  fov = 60.0_rad;

            u32  shadowMapResolution = 1024u;

            bool useBloom = true;
            bool useDepthOfField = true;
        };

        void init(const Settings& settings);
        void update(seconds deltaTime);
        void render();

        void refresh();

        void addLine(const vec3& a, const vec3& b, const vec3& c, const vec3& d = vec3(2));
        void addPoint(const vec3& pos, const vec3& color);

        void registerStaticMesh(const MeshInstance* data);
        void unregisterStaticMesh(const MeshInstance* data);

        void registerAnimMesh(const AnimatedMeshInstance* data);
        void unregisterAnimMesh(const AnimatedMeshInstance* data);

        void registerFoliage(const FoliageInstance* data);
        void unregisterFoliage(const FoliageInstance* data);

        void registerGrass(const GrassInstance* data);
        void unregisterGrass(const GrassInstance* data);

        void setActiveCamera(const Camera* camera);

        void clearAllData();

    private:
        void initShadows();
        void renderShadows();

        void prepLines();
        void renderLines();

        void prepPoints();
        void renderPoints();

    private:
        struct LinePos
        {
            vec3 a, b;
        };

        struct LineColor
        {
            vec3 c0, c1;
        };

        struct Lines
        {
            std::vector<LinePos> linePosData;
            std::vector<LineColor> lineColorData;

            VertexArray vao;
            ArrayBuffer vbo;
            ArrayBuffer cbo;
        };

        struct Points
        {
            std::vector<vec3> poses;
            std::vector<vec3> colors;

            VertexArray vao;
            ArrayBuffer vbo;
            ArrayBuffer cbo;
        };

        struct Shadows
        {
            Framebuffer fbo;

            mat4 proj;
            mat4 view;
            mat4 bvp;
            mat4 bias;
        };

        struct Shaders
        {
            Shader frame;
            Shader animMesh;
            Shader staticMesh;
            Shader foliage;
            Shader grass;
            Shader lines;
            Shader skybox;
        };

        Settings m_settings;

        mat4 m_projection;

        OrderlessArray<const MeshInstance*, 128u>         m_meshData;
        OrderlessArray<const AnimatedMeshInstance*, 128u> m_animMeshData;
        OrderlessArray<const FoliageInstance*, 128u>      m_foliageData;
        OrderlessArray<const GrassInstance*, 512u>        m_grassData;

        const Camera* m_camera = nullptr;

        seconds m_time = seconds(0);

        Shaders m_shaders;
        Shadows m_shadows;
        Skybox  m_skybox;
        Lines   m_lines;
        Points  m_points;

        Framebuffer m_fbo;
        ScreenQuad m_screenQuad;

        PostProcessBloom m_bloom;
        GLuint m_render = 0;
};

extern Renderer3D gRenderer3D;
