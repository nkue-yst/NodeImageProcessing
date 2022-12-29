#pragma once

#include <cstdint>

struct Color
{
    uint8_t r, g, b;
};

namespace NodeColor
{
    constexpr Color ImageSourceNode     { 128, 128, 255 };
    constexpr Color ImageProcessingNode {  64,  64, 128 };
    constexpr Color VideoSourceNode     { 128, 255, 128 };
    constexpr Color VideoProcessingNode {  64, 128,  64 };
}
