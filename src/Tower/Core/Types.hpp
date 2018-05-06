#pragma once
#include <stdint.h>
#include <chrono>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define M_TAU (M_PI*2)
#define HALF_PI (M_PI*0.5)

#define EPSILON (0.00001)

constexpr float operator"" _rad(long double deg)
{
    return deg * (M_PI / 180.f);
}

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int16_t;
using i8 = int8_t;

using byte = u8;

using f32 = float;
using f64 = double;

namespace chro = std::chrono;
namespace chli = std::literals::chrono_literals;

using seconds = chro::duration<float, std::ratio<1>>;

constexpr seconds operator"" _sec(long double sec)
{
    return seconds(sec);
}

constexpr seconds operator"" _ms(long double miles)
{
    return seconds(miles / 1000.0);
}

using Path = std::string;
