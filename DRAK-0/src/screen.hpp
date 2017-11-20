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

#include "color.hpp"
#include "bit_array.hpp"

namespace drak {

    class Screen {
    public:
        static constexpr int Width = 320;
        static constexpr int Height = 240;
#ifdef BIT_DEPTH_6
        static constexpr int PixelBits = 6;
#else
        static constexpr int PixelBits = 8;
#endif
        static constexpr int ScreenBytes = (Width * Height * PixelBits) / 8;

    private:
        gsl::span<unsigned char> _screen_buffer_bytes;
#ifdef BIT_DEPTH_6
        BitArray<ScreenBytes> _screen_buffer_bits;
#endif
        gsl::span<unsigned char> _palette_bytes;

        Palette _palette;

        sf::Sprite _spr;
        sf::Texture _tex;
        sf::Image _img;
        
    public:
        Screen() { } // This is an invalid state

        Screen(gsl::span<unsigned char> & screen_buffer_data, gsl::span<unsigned char> & palette_buffer_data) {
            _screen_buffer_bytes = screen_buffer_data;
#ifdef BIT_DEPTH_6
            _screen_buffer_bits = BitArray<ScreenBytes>(_screen_buffer_bytes);
#endif
            _palette_bytes = palette_buffer_data;

            memcpy(_palette.data, _palette_bytes.data(), 64 * 3);

            _img.create(Width, Height);
            _tex.loadFromImage(_img);
            _spr.setTexture(_tex);

            Draw();
        }

        Screen(Screen const& screen) {
            _screen_buffer_bytes = screen._screen_buffer_bytes;
#ifdef BIT_DEPTH_6
            _screen_buffer_bits = screen._screen_buffer_bits;
#endif
            _palette_bytes = screen._palette_bytes;
            _img = screen._img;
            _tex = screen._tex;
            _spr = screen._spr;
        }

        void SetFullPalette(Palette const& palette) {
            palette.Duplicate(_palette);
            memcpy(_palette_bytes.data(), _palette.data, sizeof(RealColor) * 64);
        }

        void SetPixel(int x, int y, int color) {
            if(x < 0 || x >= Width || y < 0 || y >= Height) return;
            int idx = (y * Width) + x;
#ifdef BIT_DEPTH_6
            _screen_buffer_bits.Set6BitsAt(idx, color);
#else
            _screen_buffer_bytes[idx] = 0b00111111 & color;
#endif
        }

        int GetPixel(int x, int y) const {
            if(x < 0 || x >= Width || y < 0 || y >= Height) return -1;
            int idx = (y * Width) + x;
#ifdef BIT_DEPTH_6
            int pxl = _screen_buffer_bits.Get6BitsAt(idx);
#else
            int pxl = _screen_buffer_bytes[idx];
#endif
            return pxl;
        }

        void Cls(int color) {
            int idx;
            for(int y = 0; y < Height; y++) {
                for(int x = 0; x < Width; x++) {
                    idx = (y * Width) + x;
#ifdef BIT_DEPTH_6
                    _screen_buffer_bits.Set6BitsAt(idx, color);
#else
                    _screen_buffer_bytes[idx] = 0b00111111 & color;
#endif
                }
            }
        }

        void SetPalette(int idx, RealColor color) {
            memcpy(_palette_bytes.data()+(idx*sizeof(RealColor)), &color, sizeof(RealColor));
            memcpy(_palette.data+(idx*sizeof(RealColor)), &color, sizeof(RealColor));
        }

        RealColor GetPalette(int idx) const {
            return _palette.data[idx];
        }

        void Draw() {
            for(int j = 0; j < Height; j++) {
                for(int i = 0; i < Width; i++) {
                    _img.setPixel(i, j, GetPalette(GetPixel(i, j)).ToSFColor());
                }
            }
        }

        void DrawOnScreen(sf::RenderTarget & target) {
            Draw();
            _tex.loadFromImage(_img);
            _spr.setTexture(_tex);
            target.draw(_spr);
        }
    };

}
