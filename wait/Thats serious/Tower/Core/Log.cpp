#include "Log.hpp"

namespace Log
{

#ifdef __unix__

void log(Type type, const char* str, ...)
{
    switch (type)
    {
        case Type::Info:
            printf(BLUE "[INFO] " RESET);
        break;
        case Type::Error:
            printf(RED "[ERROR] " RESET);
        break;
        case Type::Mem:
            printf(GREEN "[MEM] " RESET);
        break;
        case Type::Warning:
            printf(PURPLE "[WARNING] " RESET);
        break;
    }

    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}

#else

void log(Type type, const char* str, ...)
{
    switch (type)
    {
        case Type::Info:
            printf("[INFO] ");
        break;
        case Type::Error:
            printf("[ERROR] ");
        break;
    }

    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}

#endif

}
