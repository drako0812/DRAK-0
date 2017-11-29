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

#include "..\pch.h"
#include "../system.hpp"

namespace drak {

    bool System::_btn(int id) {
        return _input.IsButtonPressed(id);
    }

    bool System::_btnp(int id, int hold, int period) {
        return false;
    }

    bool System::_mbtn(int id) {
        return _input.IsMouseButtonPressed(id);
    }

    bool System::_mbtnp(int id, int hold, int period) {
        return false;
    }

    short System::_mpos(int axis) {
        return _input.GetMousePos(axis);
    }

    short System::_mposp(int axis) {
        return 0;
    }

    void System::_cls(int color) {
        _screen.Cls(color);
    }

    void System::_exit() {
        _mustQuit = true;
        throw ExitCalledException();
    }

    int System::_pix(int x, int y, int color) {
        if(color == -1) {
            return _screen.GetPixel(x, y);
        } else {
            _screen.SetPixel(x, y, color);
            return -1;
        }
    }

    int System::_time() {
        static time_type now;
        now = std::chrono::high_resolution_clock::now();
        auto diff = now - _exeStart;
        return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    }

    void System::_trace(std::string const& msg) {
        nowide::cout << msg << std::endl;
    }

    double System::_random() {
        return _rng.uniform(0.0, 1.0);
    }

    int System::_random(int n) {
        return _rng.uniform(0, n);
    }

    int System::_random(int m, int n) {
        return _rng.uniform(m, n);
    }

    void System::_memcpy(int dest, int src, int len) {
        assert((dest >= 0) && ((dest + len) < MemoryBytes) && (src >= 0) && ((src + len) < MemoryBytes) && "ERROR: memcpy out of bounds!");
        memcpy(_memory->data() + dest, _memory->data() + src, len);
    }

    void System::_memset(int dest, int val, int len) {
        assert((dest >= 0) && ((dest + len) < MemoryBytes) && "ERROR: memset out of bounds!");
        memset(_memory->data() + dest, val, len);
    }

    int System::_peek(int addr) {
        assert((addr >= 0) && (addr < MemoryBytes) && "ERROR: peek out of bounds!");
        return _memory->at(addr);
    }

    void System::_poke(int addr, int val) {
        assert((addr >= 0) && (addr < MemoryBytes) && "ERROR: poke out of bounds!");
        _memory->at(addr) = val;
    }

    // Implementation taken from: https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham%E2%80%99s-Line-Drawing-Algorithm
    void System::_line(int x0, int y0, int x1, int y1, int col) {
        bool steep = false;
        if(std::abs(x0 - x1) < std::abs(y0 - y1)) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if(x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int dx = x1 - x0;
        int dy = y1 - y0;
        int derror2 = std::abs(dy) * 2;
        int error2 = 0;
        int y = y0;
        for(int x = x0; x <= x1; x++) {
            if(steep) {
                _screen.SetPixel(y, x, col);
            } else {
                _screen.SetPixel(x, y, col);
            }
            error2 += derror2;
            if(error2 > dx) {
                y += (y1 > y0 ? 1 : -1);
                error2 -= dx * 2;
            }
        }
    }

    // Implementation taken from: https://en.wikipedia.org/wiki/Midpoint_circle_algorithm#C_example
    /*void System::_circb(int x0, int y0, int rad, int col) {
    int x = rad - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (rad << 1);

    while(x >= y) {
    _screen.SetPixel(x0 + x, y0 + y, col);
    _screen.SetPixel(x0 + y, y0 + x, col);
    _screen.SetPixel(x0 - y, y0 + x, col);
    _screen.SetPixel(x0 - x, y0 + y, col);
    _screen.SetPixel(x0 - x, y0 - y, col);
    _screen.SetPixel(x0 - y, y0 - x, col);
    _screen.SetPixel(x0 + y, y0 - x, col);
    _screen.SetPixel(x0 + x, y0 - y, col);

    if(err <= 0) {
    y++;
    err += dy;
    dy += 2;
    }
    if(err > 0) {
    x--;
    dx += 2;
    err += (-rad << 1) + dx;
    }
    }
    }*/

    void System::_circb(int x0, int y0, int rad, int col) {
        _ellipse(x0, y0, rad, rad, col);
    }

    // Based off of code from SDL from: http://www.ferzkopp.net/Software/SDL2_gfx/Docs/html/_s_d_l2__gfx_primitives_8c_source.html#l01598
    void System::_ellipse(int x, int y, int rx, int ry, int col) {
        //int result;
        int ix, iy;
        int h, i, j, k;
        int oh, oi, oj, ok;
        int xmh, xph, ypk, ymk;
        int xmi, xpi, ymj, ypj;
        int xmj, xpj, ymi, ypi;
        int xmk, xpk, ymh, yph;

        if((rx < 0) || (ry < 0)) {
            return;
        }

        if(rx == 0) {
            _line(x, y - ry, x, y + ry, col);
            return;
        }

        if(ry == 0) {
            _line(x - rx, y, x + rx, y, col);
            return;
        }

        //result = 0;

        oh = oi = oj = ok = 0xFFFF;

        if(rx > ry) {
            ix = 0;
            iy = rx * 64;

            do {
                h = (ix + 32) >> 6;
                i = (iy + 32) >> 6;
                j = (h * ry) / rx;
                k = (i * ry) / rx;

                if(((ok != k) && (oj != k)) || ((oj != j) && (ok != j)) || (k != j)) {
                    xph = x + h;
                    xmh = x - h;
                    if(k > 0) {
                        ypk = y + k;
                        ymk = y - k;
                        _pix(xmh, ypk, col);
                        _pix(xph, ypk, col);
                        _pix(xmh, ymk, col);
                        _pix(xph, ymk, col);
                    } else {
                        _pix(xmh, y, col);
                        _pix(xph, y, col);
                    }
                    ok = k;
                    xpi = x + i;
                    xmi = x - i;
                    if(j > 0) {
                        ypj = y + j;
                        ymj = y - j;
                        _pix(xmi, ypj, col);
                        _pix(xpi, ypj, col);
                        _pix(xmi, ymj, col);
                        _pix(xpi, ymj, col);
                    } else {
                        _pix(xmi, y, col);
                        _pix(xpi, y, col);
                    }
                    oj = j;
                }

                ix = ix + iy / rx;
                iy = iy + ix / rx;
            } while(i > h);
        } else {
            ix = 0;
            iy = ry * 64;

            do {
                h = (ix + 32) >> 6;
                i = (iy + 32) >> 6;
                j = (h * rx) / ry;
                k = (i * rx) / ry;

                if(((oi != i) && (oh != i)) || ((oh != h) && (oi != h) && (i != h))) {
                    xmj = x - j;
                    xpj = x + j;
                    if(i > 0) {
                        ypi = y + i;
                        ymi = y - i;
                        _pix(xmj, ypi, col);
                        _pix(xpj, ypi, col);
                        _pix(xmj, ymi, col);
                        _pix(xpj, ymi, col);
                    } else {
                        _pix(xmj, y, col);
                        _pix(xpj, y, col);
                    }
                    oi = i;
                    xmk = x - k;
                    xpk = x + k;
                    if(h > 0) {
                        yph = y + h;
                        ymh = y - h;
                        _pix(xmk, yph, col);
                        _pix(xpk, yph, col);
                        _pix(xmk, ymh, col);
                        _pix(xpk, ymh, col);
                    } else {
                        _pix(xmk, y, col);
                        _pix(xpk, y, col);
                    }
                    oh = h;
                }
                ix = ix + iy / ry;
                iy = iy - ix / ry;
            } while(i > h);
        }
    }

    // Implementation taken from: https://stackoverflow.com/a/1237519 with modifications from td-lambda's comment
    void System::_circ(int x0, int y0, int rad, int col) {
        for(int y = -rad; y <= rad; y++) {
            for(int x = -rad; x <= rad; x++) {
                if(x * x + y * y < rad * rad + rad) {
                    _screen.SetPixel(x0 + x, y0 + y, col);
                }
            }
        }
    }

    void System::_rectb(int x, int y, int w, int h, int col) {
        _line(x, y, x + w - 1, y, col);
        _line(x, y + h - 1, x + w - 1, y + h - 1, col);
        _line(x, y, x, y + h - 1, col);
        _line(x + w - 1, y, x + w - 1, y + h - 1, col);
    }

    void System::_rect(int x, int y, int w, int h, int col) {
        for(int i = 0; i < h; i++) {
            _line(x, y + i, x + w - 1, y + i, col);
        }
    }

    int System::_rgb(int r, int g, int b, int mode) {
        switch(mode) {
        case 0:
            return ((r & 0b000011) << 4) | ((g & 0b000011) << 2) | (b & 0b000011);
            break;
        case 1:
            return
                (((r & 0xFF) >> 6) << 4) |
                (((g & 0xFF) >> 6) << 2) |
                (((b & 0xFF) >> 6));
            break;
        default:
            return -1;
            break;
        }
    }

    void System::_spr(int idx, int x, int y, int ck, int scale, int flip, int rotate, int w, int h) {
        _screen.DrawSprite(idx, x, y, ck, w, h);
    }

    int System::_sidx(int p, int x, int y) {
        int idx = p * 1024;
        idx += x + y * 32;
        return idx;
    }

}
