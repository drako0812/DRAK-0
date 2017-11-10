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

        Palette Duplicate() {
            Palette tmp;
            memcpy(tmp.data, data, sizeof(RealColor) * 64);
        }
    };

    constexpr Palette DefaultPalette = {
        {
            {64,0,0},{128,0,0},{255,0,0},{128,64,64},{255,128,128}, // Red
            {64,32,0},{128,64,0},{255,128,0},{128,96,64},{255,192,128}, // Orange
            {64,64,0},{128,128,0},{255,255,0},{128,128,64},{255,255,128}, // Yellow
            {32,64,0},{64,128,0},{128,255,0},{96,128,64},{192,255,128}, // Lime
            {0,64,0},{0,128,0},{0,255,0},{64,128,64},{128,255,128}, // Green
            {0,64,32},{0,128,64},{0,255,128},{64,128,96},{128,255,192}, // Turquoise
            {0,64,64},{0,128,128},{0,255,255},{64,128,128},{128,255,255}, // Cyan
            {0,32,64},{0,64,128},{0,128,255},{64,96,128},{128,192,255}, // Sky Blue
            {0,0,64},{0,0,128},{0,0,255},{64,64,128},{128,128,255}, // Blue
            {32,0,64},{64,0,128},{128,0,255},{96,64,128},{192,128,255}, // Violet
            {64,0,64},{128,0,128},{255,0,255},{128,64,128},{255,128,255}, // Magenta
            {64,0,32},{128,0,64},{255,0,128},{128,64,96},{255,128,192}, // Pink
            {0,0,0},{84,84,84},{171,171,171},{255,255,255} // Greys
        }
    };
}
