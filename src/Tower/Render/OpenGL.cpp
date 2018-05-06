#include "OpenGL.hpp"

namespace gl
{
    void clearError()
    {
        while (glGetError() != GL_NO_ERROR);
    }

    void printError(const char* func, const char* file, int line)
    {
        while (GLenum error = glGetError())
        {
            Log::log(Log::Type::Error, "OpenGL %d in %s - %s:%d\n", error, func, file, line);
            exit(EXIT_FAILURE);
        }
    }
}
