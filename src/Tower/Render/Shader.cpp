#include "Shader.hpp"
#include "Texture.hpp"
#include "3D/Cubemap.hpp"
#include "Core/Log.hpp"
#include <fstream>

static std::string name; //FUCK YOU

struct ShaderSources
{
    std::string vertexShader;
    std::string fragmentShader;
};

Shader::~Shader()
{
    if(m_program)
    {
        GL(glDetachShader(m_program, m_vertex));
        GL(glDetachShader(m_program, m_fragment));
        GL(glDeleteShader(m_vertex));
        GL(glDeleteShader(m_fragment));
        GL(glDeleteProgram(m_program));
    }
}

auto Shader::loadSource(const char* path)
{
    std::ifstream file(path);

    if(!file.good())
    {
        Log::log(Log::Type::Error, "Could not load %s\n", path);
        exit(EXIT_FAILURE);
    }

    std::string source[2];
    std::string line;
    u32 mode = 0;

    while(!file.eof())
    {
        std::getline(file, line);

        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                mode = 0;
            else
                mode = 1;
        }
        else if (line.find("#include") != std::string::npos)
        {
            std::size_t pos = line.find_last_of('<');
            std::string incfile = line.substr(pos + 1, (line.size() - pos) - 2);

            std::string puth = "Shaders/" + incfile;

            std::ifstream inc(puth);

            if (!inc.good())
            {
                Log::log(Log::Type::Error, "Could not find include location %s in %s\n", puth.c_str(), path);
                exit(EXIT_FAILURE);
            }

            while (!inc.eof())
            {
                std::string line;
                std::getline(inc, line);

                source[mode] += line + "\n";
            }
        }
        else
            source[mode] += line + "\n";
    }

    ShaderSources sources;
    sources.vertexShader = source[0];
    sources.fragmentShader = source[1];

    return sources;
}

GLuint Shader::createShader(const std::string& source, GLuint mode)
{
    GL(GLuint id = glCreateShader(mode));

    const char* csource = source.c_str();

    GL(glShaderSource(id, 1, &csource, NULL));
    GL(glCompileShader(id));

    char error[512];
    GL(glGetShaderInfoLog(id, 512, NULL, error));
    Log::log(Log::Type::Info, "%s Comile Status: \n%s", name.c_str(), error);

    if (std::string(error).empty())
        Log::log(Log::Type::Info, "Compilation Successfull!\n");

    return id;
}

void Shader::loadFromFile(const char* path)
{
    ShaderSources sources = loadSource(path);

    name = std::string(path) + ":vertex";
    m_vertex = createShader(sources.vertexShader, GL_VERTEX_SHADER);

    name = std::string(path) + ":fragment";
    m_fragment = createShader(sources.fragmentShader, GL_FRAGMENT_SHADER);

    GL(m_program = glCreateProgram());

    GL(glAttachShader(m_program, m_vertex));
    GL(glAttachShader(m_program, m_fragment));

    GL(glLinkProgram(m_program));
    GL(glValidateProgram(m_program));
}

GLint Shader::getUniformLocation(const char* unif) const
{
    GL(GLuint location = glGetUniformLocation(m_program, unif));

    if(location == GL_INVALID_INDEX)
        printf("Location could not be found in shader %s\n", unif);

    return location;
}

void Shader::setUniformTexture(const std::string& name, u32 index, const Texture& tex) const
{
    GL(glActiveTexture(GL_TEXTURE0 + index));
    tex.bind();
    setUniform1i(name.c_str(), index);
}

void Shader::setUniformTexture(const std::string& name, u32 index, const Cubemap& tex) const
{
    GL(glActiveTexture(GL_TEXTURE0 + index));
    tex.bind();
    setUniform1i(name.c_str(), index);
}

void Shader::setUniformTexture(const std::string& name, u32 index, GLuint tex) const
{
    GL(glActiveTexture(GL_TEXTURE0 + index));
    GL(glBindTexture(GL_TEXTURE_2D, tex));
    setUniform1i(name.c_str(), index);
}

void Shader::setUniformCubemap(const std::string& name, u32 index, GLuint tex) const
{
    GL(glActiveTexture(GL_TEXTURE0 + index));
    GL(glBindTexture(GL_TEXTURE_CUBE_MAP, tex));
    setUniform1i(name.c_str(), index);
}

void Shader::setUniform1i(const char* name, i32 val) const
{
    GL(glUniform1i(getUniformLocation(name), val));
}

void Shader::setUniform1f(const char* name, float val) const
{
    GL(glUniform1f(getUniformLocation(name), val));
}

void Shader::setUniform2f(const char* name, const vec2& vec) const
{
    GL(glUniform2f(getUniformLocation(name), vec.x, vec.y));
}

void Shader::setUniform3f(const char* name, float x, float y, float z) const
{
    GL(glUniform3f(getUniformLocation(name), x, y, z));
}

void Shader::setUniform3f(const char* name, const vec3& vec) const
{
    GL(glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z));
}

void Shader::setUniform4f(const char* name, const vec4& vec) const
{
    GL(glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w));
}

void Shader::setUniformMatrix(const char* name, const mat4& mat) const
{
    GL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::bind() const
{
    GL(glUseProgram(m_program));
}

void Shader::unbind() const
{
    GL(glUseProgram(0));
}
