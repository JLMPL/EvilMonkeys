#pragma once
#include <random>

namespace Random
{
    extern std::mt19937 g_randomEngine;

    template <typename T>
    static T inRange(T min, T max)
    {
        std::uniform_real_distribution<T> range(min, max);
        return range(g_randomEngine);
    }

    template <typename T>
    static T inRangei(T min, T max)
    {
        std::uniform_int_distribution<T> range(min, max);
        return range(g_randomEngine);
    }
}
