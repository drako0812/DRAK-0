#pragma once

#include "color.hpp"
#include "bit_array.hpp"

namespace drak {

    struct ScreenBuffer {
        static constexpr int Width = 320;
        static constexpr int Height = 240;
        static constexpr int PixelBits = 6;

        BitArray<57'600> Data;

        
    };

}
