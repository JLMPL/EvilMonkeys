void SceneRenderer::renderEnvMap()
{
    Framebuffer envfra;
    GLuint size = 64;
    envfra.init(Framebuffer::Type::Color, size, size);

    m_envMap.init(size);
    m_envMap.bind();

    mat4f proj = glm::perspective(90.0_rad, 1.f, 0.5f, 2000.f);

    vec3f poz = vec3f(0.f, 20.f, 0.f);
    vec3f up = vec3f(0.f,-1.f,0.f);

    mat4f views[] =
    {
        glm::lookAt(poz, poz + vec3f(1.f,0.f,0.f), up),
        glm::lookAt(poz, poz + vec3f(-1.f,0.f,0.f), up),

        glm::lookAt(poz, poz + vec3f(0.f,1.f,0.f), vec3f(0.f,0.f,1.f)),
        glm::lookAt(poz, poz + vec3f(0.f,-1.f,0.f), vec3f(0.f,0.f,-1.f)),

        glm::lookAt(poz, poz + vec3f(0.f,0.f,1.f), up),
        glm::lookAt(poz, poz + vec3f(0.f,0.f,-1.f), up)
    };

    envfra.bind();

    for (auto i = 0; i < 6; i++)
    {
        GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_envMap.getId(), 0));
        GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        renderSkybox(proj, views[i]);
        renderStaticData(ShaderType::Low, proj, views[i]);
    }

    envfra.unbind();

    m_envMap.unbind();
}

void SceneRenderer::setUniformLights(const Shader& shader)
{
    for (auto i = 0; i < g_maxLights; i++)
    {
        vec3f pos = vec3f(0.f);
        vec3f color = vec3f(0.f);
        float intensity = 0.f;

        if (m_closestLights[i] != nullptr)
        {
            pos = m_closestLights[i]->pos;
            color = m_closestLights[i]->color;
            intensity = m_closestLights[i]->intensity;
        }

        std::string str = "light[" + std::to_string(i) + "].pos";
        shader.setUniform3f(str.c_str(), pos);

        str = "light[" + std::to_string(i) + "].color";
        shader.setUniform3f(str.c_str(), color);

        str = "light[" + std::to_string(i) + "].intensity";
        shader.setUniform1f(str.c_str(), intensity);
    }
}
