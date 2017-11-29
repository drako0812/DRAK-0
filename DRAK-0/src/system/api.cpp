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

    System & System::Get() {
        return *system;
    }

    bool System::Btn(int id) {
        return system->_btn(id);
    }

    bool System::Btnp(int id, int hold, int period) {
        return system->_btnp(id, hold, period);
    }

    bool System::Mbtn(int id) {
        return system->_mbtn(id);
    }

    bool System::Mbtnp(int id, int hold, int period) {
        return system->_mbtnp(id, hold, period);
    }

    short System::Mpos(int axis) {
        return system->_mpos(axis);
    }

    short System::Mposp(int axis) {
        return system->_mposp(axis);
    }

    void System::Cls(int color) {
        system->_cls(color);
    }

    void System::Exit() {
        system->_exit();
    }

    int System::Pix(int x, int y, int color) {
        return system->_pix(x, y, color);
    }

    int System::Time() {
        return system->_time();
    }

    void System::Trace(std::string const& msg) {
        system->_trace(msg);
    }

    double System::Random() {
        return system->_random();
    }

    int System::Random(int n) {
        return system->_random(n);
    }

    int System::Random(int m, int n) {
        return system->_random(m, n);
    }

    void System::Memcpy(int dest, int src, int len) {
        system->_memcpy(dest, src, len);
    }

    void System::Memset(int dest, int val, int len) {
        system->_memset(dest, val, len);
    }

    int System::Peek(int addr) {
        return system->_peek(addr);
    }

    void System::Poke(int addr, int val) {
        system->_poke(addr, val);
    }

    void System::Line(int x0, int y0, int x1, int y1, int col) {
        system->_line(x0, y0, x1, y1, col);
    }

    void System::Circb(int x0, int y0, int rad, int col) {
        system->_circb(x0, y0, rad, col);
    }

    void System::Circ(int x0, int y0, int rad, int col) {
        system->_circ(x0, y0, rad, col);
    }

    void System::Rectb(int x, int y, int w, int h, int col) {
        system->_rectb(x, y, w, h, col);
    }

    void System::Rect(int x, int y, int w, int h, int col) {
        system->_rect(x, y, w, h, col);
    }

    int System::Rgb(int r, int g, int b, int mode) {
        return system->_rgb(r, g, b, mode);
    }

    void System::Spr(int idx, int x, int y, int ck, int scale, int flip, int rotate, int w, int h) {
        system->_spr(idx, x, y, ck, scale, flip, rotate, w, h);
    }

    int System::Sidx(int p, int x, int y) {
        return system->_sidx(p, x, y);
    }

    void System::InitializeSystem() {
        system = std::make_shared<System>();

        // DO CHAISCRIPT

        using namespace chaiscript;

        auto & scriptEngine = system->ChaiScriptEngine();

        //scriptEngine.add(fun(&System::Scanline), "scanline");
        scriptEngine.add(fun(&System::Btn), "btn");
        scriptEngine.add(fun(&System::Btnp), "btnp");
        scriptEngine.add(fun([](int id) -> bool { return System::Btnp(id); }), "btnp");
        scriptEngine.add(fun([](int id, int hold) -> bool { return System::Btnp(id, hold); }), "btnp");
        scriptEngine.add(fun(&System::Mbtn), "mbtn");
        scriptEngine.add(fun(&System::Mbtnp), "mbtnp");
        scriptEngine.add(fun([](int id) -> bool { return System::Mbtnp(id); }), "mbtnp");
        scriptEngine.add(fun([](int id, int hold) -> bool { return System::Mbtnp(id, hold); }), "mbtnp");
        scriptEngine.add(fun(&System::Mpos), "mpos");
        scriptEngine.add(fun(&System::Mposp), "mposp");
        //scriptEngine.add(fun(&System::Clip), "clip");
        scriptEngine.add(fun(&System::Cls), "cls");
        scriptEngine.add(fun([]() { System::Cls(); }), "cls");
        scriptEngine.add(fun(&System::Circ), "circ");
        scriptEngine.add(fun(&System::Circb), "circb");
        scriptEngine.add(fun(&System::Exit), "exit");
        //scriptEngine.add(fun(&System::Font), "font");
        scriptEngine.add(fun(&System::Line), "line");
        //scriptEngine.add(fun(&System::Map), "map");
        scriptEngine.add(fun(&System::Memcpy), "memcpy");
        scriptEngine.add(fun(&System::Memset), "memset");
        //scriptEngine.add(fun(&System::Mget), "mget");
        //scriptEngine.add(fun(&System::Mouse), "mouse");
        //scriptEngine.add(fun(&System::Mset), "mset");
        //scriptEngine.add(fun(&System::Music), "music");
        scriptEngine.add(fun(&System::Peek), "peek");
        //scriptEngine.add(fun(&System::Peek4), "peek4");
        scriptEngine.add(fun(&System::Pix), "pix");
        scriptEngine.add(fun([](int x, int y) -> int { return System::Pix(x, y); }), "pix");
        //scriptEngine.add(fun(&System::Pmem), "pmem");
        scriptEngine.add(fun(&System::Poke), "poke");
        //scriptEngine.add(fun(&System::Poke4), "poke4");
        //scriptEngine.add(fun(&System::Text), "text");
        scriptEngine.add(fun(&System::Rect), "rect");
        scriptEngine.add(fun(&System::Rectb), "rectb");
        //scriptEngine.add(fun(&System::Sfx), "sfx");
        scriptEngine.add(fun(&System::Spr), "spr");
        scriptEngine.add(fun([](int idx, int x, int y, int ck, int scale, int flip, int rotate) { Spr(idx, x, y, ck, scale, flip, rotate); }), "spr");
        scriptEngine.add(fun([](int idx, int x, int y, int ck, int scale, int flip) { Spr(idx, x, y, ck, scale, flip); }), "spr");
        scriptEngine.add(fun([](int idx, int x, int y, int ck, int scale) { Spr(idx, x, y, ck, scale); }), "spr");
        scriptEngine.add(fun([](int idx, int x, int y, int ck) { Spr(idx, x, y, ck); }), "spr");
        scriptEngine.add(fun([](int idx, int x, int y) { Spr(idx, x, y); }), "spr");
        //scriptEngine.add(fun(&System::Sync), "sync");
        scriptEngine.add(fun(&System::Time), "time");
        scriptEngine.add(fun(&System::Trace), "trace");
        //scriptEngine.add(fun(&System::Tri), "tri");
        //scriptEngine.add(fun(&System::Textri), "textri");

        scriptEngine.add(fun([]() -> double { return System::Random(); }), "random");
        scriptEngine.add(fun([](int n) -> int { return System::Random(n); }), "random");
        scriptEngine.add(fun([](int m, int n) -> int { return System::Random(m, n); }), "random");
        scriptEngine.add(fun(&System::Rgb), "rgb");
        scriptEngine.add(fun([](int r, int g, int b) -> int { return System::Rgb(r, g, b); }), "rgb");
        scriptEngine.add(fun(&System::Sidx), "sidx");

        // DO LUA

        auto & lua = system->LuaScriptEngine();

        lua.open_libraries(
            sol::lib::jit,
            sol::lib::bit32,
            sol::lib::base,
            sol::lib::string,
            sol::lib::math,
            sol::lib::table,
            sol::lib::utf8
        );

        lua["btn"] = &System::Btn;
        lua["btnp"] = sol::overload(
            &System::Btnp,
            [](int id) -> bool { return System::Btnp(id); },
            [](int id, int hold) -> bool { return System::Btnp(id, hold); }
        );
        lua["mbtn"] = &System::Mbtn;
        lua["mbtnp"] = sol::overload(
            &System::Mbtnp,
            [](int id) -> bool { return System::Mbtnp(id); },
            [](int id, int hold) -> bool { return System::Mbtnp(id, hold); }
        );
        lua["mpos"] = &System::Mpos;
        lua["mposp"] = &System::Mposp;
        lua["cls"] = sol::overload(
            &System::Cls,
            []() { System::Cls(); }
        );
        lua["circ"] = &System::Circ;
        lua["circb"] = &System::Circb;
        lua["exit"] = &System::Exit;
        lua["line"] = &System::Line;
        lua["memcpy"] = &System::Memcpy;
        lua["memset"] = &System::Memset;
        lua["peek"] = &System::Peek;
        lua["pix"] = sol::overload(
            &System::Pix,
            [](int x, int y) -> int { return System::Pix(x, y); },
            [](double x, double y, int col) -> int { return System::Pix(x, y, col); },
            [](double x, double y) -> int { return System::Pix(x, y); }
        );
        lua["poke"] = &System::Poke;
        lua["rect"] = &System::Rect;
        lua["rectb"] = &System::Rectb;
        lua["spr"] = sol::overload(
            &System::Spr,
            [](int idx, int x, int y, int ck, int scale, int flip, int rotate) { Spr(idx, x, y, ck, scale, flip, rotate); },
            [](int idx, int x, int y, int ck, int scale, int flip) { Spr(idx, x, y, ck, scale, flip); },
            [](int idx, int x, int y, int ck, int scale) { Spr(idx, x, y, ck, scale); },
            [](int idx, int x, int y, int ck) { Spr(idx, x, y, ck); },
            [](int idx, int x, int y) { Spr(idx, x, y); }
        );
        lua["time"] = &System::Time;
        lua["trace"] = &System::Trace;
        lua["random"] = sol::overload(
            []() -> double { return System::Random(); },
            [](int n) -> int { return System::Random(n); },
            [](int m, int n) -> int { return System::Random(m, n); }
        );
        lua["rgb"] = sol::overload(
            &System::Rgb,
            [](int r, int g, int b) -> int { return System::Rgb(r, g, b); }
        );
        lua["sidx"] = &System::Sidx;
    }

    void System::UninitializeSystem() {
        system = nullptr;
    }

}
