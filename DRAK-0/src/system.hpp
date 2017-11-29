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

#include "bit_array.hpp"
#include "screen.hpp"
#include "input.hpp"

#include <cstdint>

namespace drak {

    class System {
    public:
        enum class ScriptLanguage : unsigned char {
            ChaiScript,
            Lua,
        };

#ifdef BIT_DEPTH_6
        static constexpr unsigned int PixelBits = 6;
#else
        static constexpr unsigned int PixelBits = 8;
#endif
        static constexpr unsigned int ScreenWidth = 320;
        static constexpr unsigned int ScreenHeight = 240;
        static constexpr unsigned int ScreenSize = (ScreenWidth * ScreenHeight * PixelBits) / 8;
        static constexpr unsigned int PaletteSize = 64 * 3;
        static constexpr unsigned int SpriteBankDim = 256;
        static constexpr unsigned int SpriteBankPageSize = (SpriteBankDim * SpriteBankDim * PixelBits) / 8;
        static constexpr unsigned int SpriteBankSize = SpriteBankPageSize * 5;
        static constexpr unsigned int MapSprites = 1200;
        static constexpr unsigned int SpriteIndexBits = 16;
        static constexpr unsigned int MapBankPageSize = (MapSprites * SpriteIndexBits) / 8;
        static constexpr unsigned int MapBankSize = MapBankPageSize * 16;
        static constexpr unsigned int ControllerSize = 16;
        static constexpr unsigned int CodeSize = 256 * 1024;
        static constexpr unsigned int StorageSize = 64 * 1024;

        static constexpr unsigned int ScreenOffset = 0;
        static constexpr unsigned int PaletteOffset = ScreenOffset + ScreenSize;
        static constexpr unsigned int SpriteBankOffset = PaletteOffset + PaletteSize;
        static constexpr unsigned int SpriteBankPageOffset0 = SpriteBankOffset;
        static constexpr unsigned int SpriteBankPageOffset1 = SpriteBankPageOffset0 + SpriteBankPageSize;
        static constexpr unsigned int SpriteBankPageOffset2 = SpriteBankPageOffset1 + SpriteBankPageSize;
        static constexpr unsigned int SpriteBankPageOffset3 = SpriteBankPageOffset2 + SpriteBankPageSize;
        static constexpr unsigned int SpriteBankPageOffset4 = SpriteBankPageOffset3 + SpriteBankPageSize;
        static constexpr unsigned int MapBankOffset = SpriteBankOffset + SpriteBankSize;
        static constexpr unsigned int MapBankPageOffset0 = MapBankOffset;
        static constexpr unsigned int MapBankPageOffset1 = MapBankPageOffset0 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset2 = MapBankPageOffset1 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset3 = MapBankPageOffset2 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset4 = MapBankPageOffset3 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset5 = MapBankPageOffset4 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset6 = MapBankPageOffset5 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset7 = MapBankPageOffset6 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset8 = MapBankPageOffset7 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset9 = MapBankPageOffset8 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset10 = MapBankPageOffset9 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset11 = MapBankPageOffset10 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset12 = MapBankPageOffset11 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset13 = MapBankPageOffset12 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset14 = MapBankPageOffset13 + MapBankPageSize;
        static constexpr unsigned int MapBankPageOffset15 = MapBankPageOffset14 + MapBankPageSize;
        static constexpr unsigned int ControllerOffset = MapBankOffset + MapBankSize;
        static constexpr unsigned int LanguageOffset = ControllerOffset + ControllerSize;
        static constexpr unsigned int CodeOffset = LanguageOffset + 1;
        static constexpr unsigned int StorageOffset = CodeOffset + CodeSize;

        static constexpr unsigned int MemoryBytes =
            ScreenSize +
            PaletteSize +
            SpriteBankSize +
            MapBankSize +
            ControllerSize +
            1 +
            CodeSize +
            StorageSize;


    private:
        using array_type = std::array<unsigned char, MemoryBytes>;
        using array_ptr = std::shared_ptr<array_type>;
        array_ptr _memory;
        BitArray<MemoryBytes> _bits;

        ScriptLanguage _lang;
        chaiscript::ChaiScript _chai_scriptEngine;
        sol::state _lua_scriptEngine;
        bool _mustQuit;
        
        using time_type = decltype(std::chrono::high_resolution_clock::now());
        
        time_type _exeStart;

        Screen _screen;
        Input _input;

        randutils::mt19937_rng _rng;

        static std::shared_ptr<System> system;
    public:
    
        class ExitCalledException {};
        
        System();
        void SetLanguage(ScriptLanguage lang);
        ScriptLanguage GetLanguage() const;
        chaiscript::ChaiScript & ChaiScriptEngine();
        sol::state & LuaScriptEngine();
        Screen & Screen();
        void Update(sf::RenderWindow & window, sf::View const& view);
        bool HasUpdate();
        bool MustQuit();
        void Quit();
        bool LoadScript(ScriptLanguage lang, std::string const& source);
        bool ImportSprites(std::string const& source);

        // These functions a bound to the scripting API
        //
        // scanline
        // btn
        // btnp
        // clip
        // cls
        // circ
        // circb
        // exit
        // font
        // line
        // map
        // memcpy
        // memset
        // mget
        // mouse
        // mset
        // music -- N/A
        // peek
        // peek4
        // pix
        // pmem
        // poke
        // poke4
        // text
        // rect
        // rectb
        // sfx -- N/A
        // spr
        // sync
        // time
        // trace
        // tri
        // textri

        // random

        bool _btn(int id);
        bool _btnp(int id, int hold = -1, int period = -1);
        bool _mbtn(int id);
        bool _mbtnp(int id, int hold = -1, int period = -1);
        short _mpos(int axis);
        short _mposp(int axis);
        void _cls(int color = 0);
        void _exit();
        int _pix(int x, int y, int color = -1);
        int _time();
        void _trace(std::string const& msg);
        double _random();
        int _random(int n);
        int _random(int m, int n);
        void _memcpy(int dest, int src, int len);
        void _memset(int dest, int val, int len);
        int _peek(int addr);
        void _poke(int addr, int val);
        void _line(int x0, int y0, int x1, int y1, int col);
        void _circb(int x0, int y0, int rad, int col);
        void _ellipse(int x, int y, int rx, int ry, int col);
        void _circ(int x0, int y0, int rad, int col);
        void _rectb(int x, int y, int w, int h, int col);
        void _rect(int x, int y, int w, int h, int col);
        int _rgb(int r, int g, int b, int mode = 0);
        void _spr(int idx, int x, int y, int ck = -1, int scale = 1, int flip = 0, int rotate = 0, int w = 1, int h = 1);
        int _sidx(int p, int x, int y);

        static System & Get();
        static bool Btn(int id);
        static bool Btnp(int id, int hold = -1, int period = -1);
        static bool Mbtn(int id);
        static bool Mbtnp(int id, int hold = -1, int period = -1);
        static short Mpos(int axis);
        static short Mposp(int axis);
        static void Cls(int color = 0);
        static void Exit();
        static int Pix(int x, int y, int color = -1);
        static int Time();
        static void Trace(std::string const& msg);
        static double Random();
        static int Random(int n);
        static int Random(int m, int n);
        static void Memcpy(int dest, int src, int len);
        static void Memset(int dest, int val, int len);
        static int Peek(int addr);
        static void Poke(int addr, int val);
        static void Line(int x0, int y0, int x1, int y1, int col);
        static void Circb(int x0, int y0, int rad, int col);
        static void Circ(int x0, int y0, int rad, int col);
        static void Rectb(int x, int y, int w, int h, int col);
        static void Rect(int x, int y, int w, int h, int col);
        static int Rgb(int r, int g, int b, int mode = 0);
        static void Spr(int idx, int x, int y, int ck = -1, int scale = 1, int flip = 0, int rotate = 0, int w = 1, int h = 1);
        static int Sidx(int p, int x, int y);
        static void InitializeSystem();
        static void UninitializeSystem();

    };

}
