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
        static constexpr unsigned int SpriteBankPageSize = (SpriteBankDim * SpriteBankDim * 6) / 8;
        static constexpr unsigned int SpriteBankSize = SpriteBankPageSize * 5;
        static constexpr unsigned int MapSprites = 1200;
        static constexpr unsigned int SpriteIndexBits = 13;
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
    
        System() : _mustQuit{false}, _memory{std::make_shared<array_type>()} {
            _lang = ScriptLanguage::ChaiScript;
            _bits = BitArray<MemoryBytes>(gsl::span<unsigned char>{*_memory});
            auto * _screen_ptr = _memory->data();
            auto * _palette_ptr = _memory->data() + PaletteOffset;
            auto _screen_span = gsl::span<unsigned char>{_screen_ptr, static_cast<std::size_t>(ScreenSize)};
            auto _palette_span = gsl::span<unsigned char>{_palette_ptr, static_cast<std::size_t>(PaletteSize)};
            _screen = drak::Screen(_screen_span, _palette_span);
            _screen.SetFullPalette(DefaultPalette);
            auto * _input_ptr = _memory->data() + ControllerOffset;
            auto _input_span = gsl::span<unsigned char>{_input_ptr, static_cast<std::size_t>(ControllerSize)};
            _input = drak::Input(_input_span);
        }

        void SetLanguage(ScriptLanguage lang) {
            _lang = lang;
            _memory->at(LanguageOffset) = static_cast<unsigned char>(lang);
        }

        ScriptLanguage GetLanguage() const {
            return _lang;
        }

        chaiscript::ChaiScript & ChaiScriptEngine() {
            return _chai_scriptEngine;
        }

        sol::state & LuaScriptEngine() {
            return _lua_scriptEngine;
        }

        Screen & Screen() {
            return _screen;
        }

        void Update(sf::RenderWindow & window, sf::View const& view) {
            _input.Update(window, view);
            switch(_lang) {
            case ScriptLanguage::ChaiScript:
                _chai_scriptEngine.eval("update();");
                break;
            case ScriptLanguage::Lua:
                _lua_scriptEngine["update"]();
                break;
            }
        }
        
        bool HasUpdate() {
            bool exists = false;
            switch(_lang) {
            case ScriptLanguage::ChaiScript:
                exists = _chai_scriptEngine.eval<bool>("call_exists(update)");
                break;
            case ScriptLanguage::Lua:
            {
                sol::optional<sol::function> is_func = _lua_scriptEngine["update"];
                exists = is_func ? true : false;
                break;
            }
            }
            return exists;
        }

        bool MustQuit() {
            return _mustQuit;
        }

        void Quit() {
            _mustQuit = true;
        }

        bool LoadScript(ScriptLanguage lang, std::string const& source) {
            SetLanguage(lang);
            nowide::cout << "Source size = " << source.length() << std::endl;
            assert((source.length() <= CodeSize) && "ERROR: Code is too big! Maximum of 256 KiB or 262144 Bytes.");
            //strncpy((char *)(_memory->data()), source.c_str(), CodeSize);
            strncpy_s((char *)(_memory->data() + CodeOffset), CodeSize, source.c_str(), CodeSize);
            _exeStart = std::chrono::high_resolution_clock::now();
            switch(lang) {
            case ScriptLanguage::ChaiScript:
                try {
                    _chai_scriptEngine.eval(source);
                } catch(ExitCalledException const& ex) {
                    // Do nothing
                }
                break;
            case ScriptLanguage::Lua:
                try {
                    _lua_scriptEngine.script(source);
                } catch(ExitCalledException const& ex) {
                    // Do nothing
                } catch(sol::error const& ex) {
                    nowide::cout << "ERROR: \n"
                        << ex.what() << std::endl;
                    throw;
                }
                break;
            }
            return HasUpdate();
        }

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

        bool _btn(int id) {
            return _input.IsButtonPressed(id);
        }

        bool _btnp(int id, int hold = -1, int period = -1) {
            return false;
        }

        bool _mbtn(int id) {
            return _input.IsMouseButtonPressed(id);
        }

        bool _mbtnp(int id, int hold = -1, int period = -1) {
            return false;
        }

        short _mpos(int axis) {
            return _input.GetMousePos(axis);
        }

        short _mposp(int axis) {
            return 0;
        }

        void _cls(int color = 0) {
            _screen.Cls(color);
        }

        void _exit() {
            _mustQuit = true;
            throw ExitCalledException();
        }

        int _pix(int x, int y, int color = -1) {
            if(color == -1) {
                return _screen.GetPixel(x, y);
            } else {
                _screen.SetPixel(x, y, color);
                return -1;
            }
        }

        int _time() {
            static time_type now;
            now = std::chrono::high_resolution_clock::now();
            auto diff = now - _exeStart;
            return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
        }

        void _trace(std::string const& msg) {
            nowide::cout << msg << std::endl;
        }

        double _random() {
            return _rng.uniform(0.0, 1.0);
        }

        int _random(int n) {
            return _rng.uniform(0, n);
        }

        int _random(int m, int n) {
            return _rng.uniform(m, n);
        }

        static System & Get() {
            return *system;
        }

        static bool Btn(int id) {
            return system->_btn(id);
        }

        static bool Btnp(int id, int hold=-1, int period=-1) {
            return system->_btnp(id, hold, period);
        }

        static bool Mbtn(int id) {
            return system->_mbtn(id);
        }

        static bool Mbtnp(int id, int hold = -1, int period = -1) {
            return system->_mbtnp(id, hold, period);
        }

        static short Mpos(int axis) {
            return system->_mpos(axis);
        }

        static short Mposp(int axis) {
            return system->_mposp(axis);
        }

        static void Cls(int color=0) {
            system->_cls(color);
        }

        static void Exit() {
            system->_exit();
        }

        static int Pix(int x, int y, int color = -1) {
            return system->_pix(x, y, color);
        }

        static int Time() {
            return system->_time();
        }

        static void Trace(std::string const& msg) {
            system->_trace(msg);
        }

        static double Random() {
            return system->_random();
        }

        static int Random(int n) {
            return system->_random(n);
        }

        static int Random(int m, int n) {
            return system->_random(m, n);
        }

        static void InitializeSystem() {
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
            //scriptEngine.add(fun(&System::Circ), "circ");
            //scriptEngine.add(fun(&System::Circb), "circb");
            scriptEngine.add(fun(&System::Exit), "exit");
            //scriptEngine.add(fun(&System::Font), "font");
            //scriptEngine.add(fun(&System::Line), "line");
            //scriptEngine.add(fun(&System::Map), "map");
            //scriptEngine.add(fun(&System::Memcpy), "memcpy");
            //scriptEngine.add(fun(&System::Memset), "memset");
            //scriptEngine.add(fun(&System::Mget), "mget");
            //scriptEngine.add(fun(&System::Mouse), "mouse");
            //scriptEngine.add(fun(&System::Mset), "mset");
            //scriptEngine.add(fun(&System::Music), "music");
            //scriptEngine.add(fun(&System::Peek), "peek");
            //scriptEngine.add(fun(&System::Peek4), "peek4");
            scriptEngine.add(fun(&System::Pix), "pix");
            scriptEngine.add(fun([](int x, int y) -> int { return System::Pix(x, y); }), "pix");
            //scriptEngine.add(fun(&System::Pmem), "pmem");
            //scriptEngine.add(fun(&System::Poke), "poke");
            //scriptEngine.add(fun(&System::Poke4), "poke4");
            //scriptEngine.add(fun(&System::Text), "text");
            //scriptEngine.add(fun(&System::Rect), "rect");
            //scriptEngine.add(fun(&System::Rectb), "rectb");
            //scriptEngine.add(fun(&System::Sfx), "sfx");
            //scriptEngine.add(fun(&System::Spr), "spr");
            //scriptEngine.add(fun(&System::Sync), "sync");
            scriptEngine.add(fun(&System::Time), "time");
            scriptEngine.add(fun(&System::Trace), "trace");
            //scriptEngine.add(fun(&System::Tri), "tri");
            //scriptEngine.add(fun(&System::Textri), "textri");

            scriptEngine.add(fun([]() -> double { return System::Random(); }), "random");
            scriptEngine.add(fun([](int n) -> int { return System::Random(n); }), "random");
            scriptEngine.add(fun([](int m, int n) -> int { return System::Random(m, n); }), "random");

            // DO LUA

            auto & lua = system->LuaScriptEngine();

            lua.open_libraries(
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
            lua["exit"] = &System::Exit;
            lua["pix"] = sol::overload(
                &System::Pix,
                [](int x, int y) -> int { return System::Pix(x, y); }
            );
            lua["time"] = &System::Time;
            lua["trace"] = &System::Trace;
            lua["random"] = sol::overload(
                []() -> double { return System::Random(); },
                [](int n) -> int { return System::Random(n); },
                [](int m, int n) -> int { return System::Random(m, n); }
            );
        }

        static void UnitializeSystem() {
            system = nullptr;
        }
    };

}
