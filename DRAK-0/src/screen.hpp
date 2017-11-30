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

        static constexpr int SpritesPerBank = 1024;

    private:
        gsl::span<unsigned char> _screen_buffer_bytes;
#ifdef BIT_DEPTH_6
        BitArray<ScreenBytes> _screen_buffer_bits;
#endif
        gsl::span<unsigned char> _palette_bytes;
        gsl::span<unsigned char> _sprite_bytes;
#ifdef BIT_DEPTH_6
        BitArray<System::SpriteBankSize> _sprite_bits;
#endif

        Palette _palette;

        sf::Sprite _spr;
        sf::Texture _tex;
        sf::Image _img;
        
    public:
        Screen() { } // This is an invalid state

        Screen(gsl::span<unsigned char> & screen_buffer_data, gsl::span<unsigned char> & palette_buffer_data, gsl::span<unsigned char> & sprite_bank_data) {
            _screen_buffer_bytes = screen_buffer_data;
#ifdef BIT_DEPTH_6
            _screen_buffer_bits = BitArray<ScreenBytes>(_screen_buffer_bytes);
#endif
            _palette_bytes = palette_buffer_data;
            _sprite_bytes = sprite_bank_data;
#ifdef BIT_DEPTH_6
            _sprite_bits = BitArray<System::SpriteBankSize>(_sprite_bytes);
#endif

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
            _sprite_bytes = screen._sprite_bytes;
#ifdef BIT_DEPTH_6
            _sprite_bits = screen._sprite_bits;
#endif

            _img = screen._img;
            _tex = screen._tex;
            _spr = screen._spr;
        }

        void SetFullPalette(Palette const& palette) {
            palette.Duplicate(_palette);
            memcpy(_palette_bytes.data(), _palette.data, sizeof(RealColor) * 64);
        }

        void SetPixel(int x, int y, int color) {
            if(color < 0) return;
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

        // Basic function
        void DrawSprite(int idx, int x, int y, int ck = -1, int w = 1, int h = 1) {
            int page = idx / SpritesPerBank;
            int page_off = idx % SpritesPerBank;
            int page_x = page_off % 32;
            int page_y = ((page * 256) + (page_off / 32)) + 7;
            for(int sy = 0, scy = y; sy < w * 8; sy++, scy++) {
                for(int sx = 0, scx = x; sx < h * 8; sx++, scx++) {
                    int col = GetSpriteBankPixel(page_x + sx, page_y + sy);
                    SetPixel(scx, scy, (col == ck) ? -1 : col);
                }
            }
        }

        int GetSpriteBankPixel(int x, int y) {
            if(x < 0 || x >= 256 || y < 0 || y >= 256 * 5) return -1;
            int idx = (y * 256) + x;
#ifdef BIT_DEPTH_6
            int pxl = _sprite_bank_bits.Get6BitsAt(idx);
#else
            int pxl = _sprite_bytes[idx];
#endif
            return pxl;
        }

        void Draw() {
            // Always make sure palette is synced
            memcpy(_palette.data, _palette_bytes.data(), 64 * 3);

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
