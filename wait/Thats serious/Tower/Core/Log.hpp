#pragma once
#include <stdio.h>
#include <stdarg.h>

#define RED     "\x1B[1;31m"
#define GREEN   "\x1B[1;32m"
#define YELLOW  "\x1B[1;33m"
#define BLUE    "\x1B[1;34m"
#define MAGENTA "\x1B[1;35m"
#define PURPLE  "\x1B[0;35m"
#define CYAN    "\x1B[1;36m"
#define WHITE   "\x1B[1;37m"
#define RESET   "\x1B[1;0m"

namespace Log
{

enum Type
{
    Info,
    Error,
    Warning,
    Mem
};

void log(Type type, const char* str, ...);

}
