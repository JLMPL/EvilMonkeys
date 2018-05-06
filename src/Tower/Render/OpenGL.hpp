#pragma once
#include <GL/glew.h>
#include "Core/Log.hpp"
#include <stdlib.h>

#define TEST_MODE 1

#if TEST_MODE == 1

#define GL(x) gl::clearError();\
x;\
gl::printError(#x, __FILE__, __LINE__);

#else
#define GL(x) x
#endif

namespace gl
{
    extern void clearError();
    extern void printError(const char* func, const char* file, int line);
}
