// The MIT License (MIT)
//
// Copyright (c) 2017 Drew Wibbenmeyer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

#include <SFML/Graphics.hpp>

namespace drak {

    struct Color {
        unsigned char idx : 6;
    };

    struct RealColor {
        unsigned char r;
        unsigned char g;
        unsigned char b;

        sf::Color ToSFColor() {
            return sf::Color(r, g, b);
        }

        static RealColor FromSFColor(sf::Color const& col) {
            return RealColor{col.r, col.g, col.b};
        }
    };

    struct Palette {
        RealColor data[64];
        //std::array<RealColor, 64> data;

        void Duplicate(Palette & other) const {
            memcpy(other.data, data, sizeof(RealColor) * 64);
        }
    };

    constexpr Palette DefaultPalette = {
        {
            {0,0,0},{84,84,84},{171,171,171},{255,255,255}, // Greys (0-3)
            {64,0,0},{128,0,0},{255,0,0},{128,64,64},{255,128,128}, // Red (4-8)
            {64,32,0},{128,64,0},{255,128,0},{128,96,64},{255,192,128}, // Orange (9-13)
            {64,64,0},{128,128,0},{255,255,0},{128,128,64},{255,255,128}, // Yellow (14-18)
            {32,64,0},{64,128,0},{128,255,0},{96,128,64},{192,255,128}, // Lime (19-23)
            {0,64,0},{0,128,0},{0,255,0},{64,128,64},{128,255,128}, // Green (24-28)
            {0,64,32},{0,128,64},{0,255,128},{64,128,96},{128,255,192}, // Turquoise (29-33)
            {0,64,64},{0,128,128},{0,255,255},{64,128,128},{128,255,255}, // Cyan (34-38)
            {0,32,64},{0,64,128},{0,128,255},{64,96,128},{128,192,255}, // Sky Blue (39-43)
            {0,0,64},{0,0,128},{0,0,255},{64,64,128},{128,128,255}, // Blue (44-48)
            {32,0,64},{64,0,128},{128,0,255},{96,64,128},{192,128,255}, // Violet (49-53)
            {64,0,64},{128,0,128},{255,0,255},{128,64,128},{255,128,255}, // Magenta (54-58)
            {64,0,32},{128,0,64},{255,0,128},{128,64,96},{255,128,192} // Pink (59-63)
        }
    };
}
