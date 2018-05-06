#include "Renderer3D.hpp"
#include "3D/MeshInstance.hpp"
#include "3D/AnimatedMeshInstance.hpp"
#include "3D/GrassInstance.hpp"
#include "3D/FoliageInstance.hpp"
#include "3D/Mesh.hpp"
#include "3D/Material.hpp"
#include "Framebuffer.hpp"
#include "3D/Camera.hpp"
#include "Core/Config.hpp"
#include <stb/stb_image.h>

Renderer3D gRenderer3D;

void Renderer3D::initShadows()
{
    float size = 50.f;
    vec3 dir = vec3(1,1,1);

    m_shadows.bias = mat4(
        0.5f, 0.f,  0.f,  0.f,
        0.f,  0.5f, 0.f,  0.f,
        0.f,  0.f,  0.5f, 0.f,
        0.5f, 0.5f, 0.5f, 1.f
    );

    m_shadows.fbo.init(Framebuffer::Type::Depth, 2048, 2048);
    m_shadows.proj = math::ortho(-size, size, -size, size, 0.1f, 1000.f);
    m_shadows.view = math::lookAt(dir * 300.f, vec3(0), math::vecY);
    m_shadows.bvp = m_shadows.bias * (m_shadows.proj * m_shadows.view);
}

void Renderer3D::init(const Settings& settings)
{
    m_settings = settings;

    m_projection = math::perspective(
        m_settings.fov,
        float(Config::Display::Width)/float(Config::Display::Height),
        0.01f,
        2000.f
    );

    m_shaders.frame.loadFromFile("Shaders/Frame.sha");
    m_shaders.animMesh.loadFromFile("Shaders/Animated/AnimatedBase.sha");
    m_shaders.staticMesh.loadFromFile("Shaders/Static/StaticBase.sha");
    m_shaders.foliage.loadFromFile("Shaders/Foliage/FoliageBase.sha");
    m_shaders.grass.loadFromFile("Shaders/Grass/GrassBase.sha");
    m_shaders.lines.loadFromFile("Shaders/DebugLines.sha");
    m_shaders.skybox.loadFromFile("Shaders/Skybox.sha");

    m_fbo.init(Framebuffer::Type::Color, Config::Display::Width, Config::Display::Height);
    m_screenQuad.init();

    m_skybox.init("miramar");

    initShadows();

    m_bloom.init();

    initColorPalette();
}

void Renderer3D::initColorPalette()
{
    i32 w, h, channels;
    byte* image = stbi_load("Textures/colors.png", &w, &h, &channels, 4);

    if (!image)
    {
        Log::log(Log::Type::Error, "Could not load palette\n");
        exit(EXIT_FAILURE);
    }

    GL(glGenTextures(1, &m_colorPalette));
    GL(glBindTexture(GL_TEXTURE_3D, m_colorPalette));

    GL(glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, 16, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, image));

    GL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP));
    GL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP));
    GL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP));
    GL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    stbi_image_free(image);

    GL(glBindTexture(GL_TEXTURE_3D, 0));
}

void Renderer3D::addLine(const vec3& a, const vec3& b, const vec3& c, const vec3& d)
{
    m_lines.linePosData.push_back({a,b});

    vec3 ce = d;

    if (ce == vec3(2))
        ce = c;

    m_lines.lineColorData.push_back({c,ce});
}

void Renderer3D::addPoint(const vec3& pos, const vec3& color)
{
    m_points.poses.push_back(pos);
    m_points.colors.push_back(color);
}

void Renderer3D::registerStaticMesh(const MeshInstance* data)
{
    m_meshData.add(data);
}

void Renderer3D::unregisterStaticMesh(const MeshInstance* data)
{
    m_meshData.remove(data);
}

void Renderer3D::registerAnimMesh(const AnimatedMeshInstance* data)
{
    m_animMeshData.add(data);
}

void Renderer3D::unregisterAnimMesh(const AnimatedMeshInstance* data)
{
    m_animMeshData.remove(data);
}

void Renderer3D::registerFoliage(const FoliageInstance* data)
{
    m_foliageData.add(data);
}

void Renderer3D::unregisterFoliage(const FoliageInstance* data)
{
    m_foliageData.remove(data);
}

void Renderer3D::registerGrass(const GrassInstance* data)
{
    m_grassData.add(data);
}

void Renderer3D::unregisterGrass(const GrassInstance* data)
{
    m_grassData.remove(data);
}

void Renderer3D::update(seconds deltaTime)
{
    m_time += deltaTime;
}

void Renderer3D::prepLines()
{
    m_lines.vao.init();
    m_lines.vao.bind();

    m_lines.vbo.init(GL_ARRAY_BUFFER);
    m_lines.vbo.bind();
    m_lines.vbo.setData(sizeof(vec3) * 2 * m_lines.linePosData.size(), &m_lines.linePosData[0], GL_STATIC_DRAW);
    m_lines.vao.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_lines.cbo.init(GL_ARRAY_BUFFER);
    m_lines.cbo.bind();
    m_lines.cbo.setData(sizeof(vec3) * 2 * m_lines.lineColorData.size(), &m_lines.lineColorData[0], GL_STATIC_DRAW);
    m_lines.vao.vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_lines.vao.unbind();
    m_lines.vbo.unbind();
}

void Renderer3D::renderLines()
{
    m_shaders.lines.bind();
    m_shaders.lines.setUniformMatrix("uProj", m_projection);
    m_shaders.lines.setUniformMatrix("uView", m_camera->getView());

    m_lines.vao.bind();
    GL(glDrawArrays(GL_LINES, 0, m_lines.linePosData.size() * 2));
    m_lines.vao.unbind();

    m_shaders.lines.unbind();
}

void Renderer3D::prepPoints()
{
    m_points.vao.init();
    m_points.vao.bind();

    m_points.vbo.init(GL_ARRAY_BUFFER);
    m_points.vbo.bind();
    m_points.vbo.setData(sizeof(vec3) * m_points.poses.size(), &m_points.poses[0], GL_STATIC_DRAW);
    m_points.vao.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_points.cbo.init(GL_ARRAY_BUFFER);
    m_points.cbo.bind();
    m_points.cbo.setData(sizeof(vec3) * m_points.colors.size(), &m_points.colors[0], GL_STATIC_DRAW);
    m_points.vao.vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    m_points.vao.unbind();
    m_points.vbo.unbind();
}

void Renderer3D::renderPoints()
{
    m_shaders.lines.bind();
    m_shaders.lines.setUniformMatrix("uProj", m_projection);
    m_shaders.lines.setUniformMatrix("uView", m_camera->getView());

    m_points.vao.bind();
    GL(glDrawArrays(GL_POINTS, 0, m_points.poses.size()));
    m_points.vao.unbind();

    m_shaders.lines.unbind();
}

void Renderer3D::renderShadows()
{
    GL(glDisable(GL_CULL_FACE));
    m_shadows.fbo.bind();
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_shaders.staticMesh.bind();
    m_shaders.staticMesh.setUniformMatrix("uProj", m_shadows.proj);
    m_shaders.staticMesh.setUniformMatrix("uView", m_shadows.view);

    for (u32 i = 0; i < m_meshData.size(); i++)
    {
        m_shaders.staticMesh.setUniformMatrix("uModel", m_meshData[i]->transform);
        m_meshData[i]->mesh->render(0);
    }

    m_shaders.staticMesh.unbind();

    //anim
    m_shaders.animMesh.bind();
    m_shaders.animMesh.setUniformMatrix("uProj", m_shadows.proj);
    m_shaders.animMesh.setUniformMatrix("uView", m_shadows.view);

    for (u32 i = 0; i < m_animMeshData.size(); i++)
    {
        m_shaders.animMesh.setUniformMatrix("uModel", m_animMeshData[i]->m_transform);
        m_animMeshData[i]->render(m_shaders.animMesh);
    }

    m_shaders.animMesh.unbind();

    //foliage
    m_shaders.foliage.bind();
    m_shaders.foliage.setUniformMatrix("uProj", m_shadows.proj);
    m_shaders.foliage.setUniformMatrix("uView", m_shadows.view);

    for (u32 i = 0; i < m_foliageData.size(); i++)
    {
        m_shaders.foliage.setUniformMatrix("uModel", m_foliageData[i]->transform);
        m_shaders.foliage.setUniform1f("time", m_time.count());
        m_foliageData[i]->mesh->render(0);
    }

    m_shaders.foliage.unbind();

    m_shadows.fbo.unbind();
    GL(glEnable(GL_CULL_FACE));
}

void Renderer3D::render()
{
    renderShadows();
    prepLines();
    prepPoints();

    m_fbo.bind();
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_shaders.skybox.bind();
    m_shaders.skybox.setUniformMatrix("uProj", m_projection);
    m_shaders.skybox.setUniformMatrix("uView", m_camera->getView());

    m_skybox.render(true);
    m_shaders.skybox.unbind();

    renderLines();
    renderPoints();

    //static
    m_shaders.staticMesh.bind();
    m_shaders.staticMesh.setUniformMatrix("uProj", m_projection);
    m_shaders.staticMesh.setUniformMatrix("uView", m_camera->getView());
    m_shaders.staticMesh.setUniformMatrix("uShadow", m_shadows.bvp);
    m_shaders.staticMesh.setUniformTexture("uShadowMap", 1, m_shadows.fbo.getDepth());

    for (u32 i = 0; i < m_meshData.size(); i++)
    {
        m_shaders.staticMesh.setUniformMatrix("uModel", m_meshData[i]->transform);

        if (m_meshData[i]->material)
        {
            m_shaders.staticMesh.setUniformTexture("uImage", 0, *m_meshData[i]->material->m_diffuse);
            m_shaders.staticMesh.setUniformTexture("uNormal", 2, *m_meshData[i]->material->m_normal);
        }

        m_meshData[i]->mesh->render(0);
    }

    m_shaders.staticMesh.unbind();

    //anim
    m_shaders.animMesh.bind();
    m_shaders.animMesh.setUniformMatrix("uProj", m_projection);
    m_shaders.animMesh.setUniformMatrix("uView", m_camera->getView());
    m_shaders.animMesh.setUniformMatrix("uShadow", m_shadows.bvp);
    m_shaders.animMesh.setUniformTexture("uShadowMap", 0, m_shadows.fbo.getDepth());

    for (u32 i = 0; i < m_animMeshData.size(); i++)
    {
        m_shaders.animMesh.setUniformMatrix("uModel", m_animMeshData[i]->m_transform);
        m_animMeshData[i]->render(m_shaders.animMesh);
    }

    m_shaders.animMesh.unbind();

    //foliage
    m_shaders.foliage.bind();
    m_shaders.foliage.setUniformMatrix("uProj", m_projection);
    m_shaders.foliage.setUniformMatrix("uView", m_camera->getView());
    m_shaders.foliage.setUniformMatrix("uShadow", m_shadows.bvp);
    m_shaders.foliage.setUniformTexture("uShadowMap", 1, m_shadows.fbo.getDepth());

    for (u32 i = 0; i < m_foliageData.size(); i++)
    {
        m_shaders.foliage.setUniformMatrix("uModel", m_foliageData[i]->transform);
        m_shaders.foliage.setUniform1f("time", m_time.count());

        if (m_foliageData[i]->material)
            m_shaders.foliage.setUniformTexture("uImage", 0, *m_foliageData[i]->material->m_diffuse);

        m_foliageData[i]->mesh->render(0);
    }

    m_shaders.foliage.unbind();

    //grass

    GL(glDisable(GL_CULL_FACE));
    m_shaders.grass.bind();
    m_shaders.grass.setUniformMatrix("uProj", m_projection);
    m_shaders.grass.setUniformMatrix("uView", m_camera->getView());
    m_shaders.grass.setUniformMatrix("uShadow", m_shadows.bvp);
    m_shaders.grass.setUniformTexture("uShadowMap", 1, m_shadows.fbo.getDepth());

    for (u32 i = 0; i < m_grassData.size(); i++)
    {
        m_shaders.grass.setUniformMatrix("uModel", m_grassData[i]->m_transform);
        m_shaders.grass.setUniform1f("time", m_time.count());

        if (m_grassData[i]->m_material)
            m_shaders.grass.setUniformTexture("uImage", 0, *m_grassData[i]->m_material->m_diffuse);

        m_grassData[i]->render();
    }

    m_shaders.grass.unbind();
    GL(glEnable(GL_CULL_FACE));
    m_fbo.unbind();

    m_render = m_bloom.process(m_fbo.getTexture());

    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_shaders.frame.bind();
    m_shaders.frame.setUniformTexture("image", 0, m_render);
    m_screenQuad.render();
    m_shaders.frame.unbind();

    m_lines.linePosData.clear();
    m_lines.lineColorData.clear();

    m_points.poses.clear();
    m_points.colors.clear();
}

void Renderer3D::setActiveCamera(const Camera* camera)
{
    m_camera = camera;
}

void Renderer3D::clearAllData()
{
    m_meshData.clear();
    m_animMeshData.clear();
    m_foliageData.clear();
    m_grassData.clear();
}

// it's quarter after one
// im the one need you now
// i just need you now
