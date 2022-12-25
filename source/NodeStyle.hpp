#pragma once

#include <cstdint>

struct Color
{
    uint8_t r, g, b;
};

namespace NodeColor
{
    constexpr Color ImageSourceNode     { 255, 128, 128 };
    constexpr Color ImageProcessingNode { 128, 128, 255 };
}
