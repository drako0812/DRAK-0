#pragma once

#include "bit_array.hpp"

namespace drak {

    class System {

        static constexpr unsigned int ScreenSize = (320 * 240 * 6) / 8;
        static constexpr unsigned int SpriteBankPageSize = (256 * 256 * 6) / 8;
        static constexpr unsigned int SpriteBankSize = SpriteBankPageSize * 5;
        static constexpr unsigned int MapSprites = 1200;
        static constexpr unsigned int SpriteIndexBits = 13;
        static constexpr unsigned int MapBankPageSize = (MapSprites * SpriteIndexBits) / 8;
        static constexpr unsigned int MapBankSize = MapBankPageSize * 16;
        static constexpr unsigned int ControllerSize = 16;
        static constexpr unsigned int CodeSize = 256 * 1024;
        static constexpr unsigned int StorageSize = 64 * 1024;

        static constexpr unsigned int ScreenOffset = 0;
        static constexpr unsigned int SpriteBankOffset = ScreenOffset + ScreenSize;
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
        static constexpr unsigned int CodeOffset = ControllerOffset + ControllerSize;
        static constexpr unsigned int StorageOffset = CodeOffset + CodeSize;

        static constexpr unsigned int MemoryBytes =
            ScreenSize +
            SpriteBankSize +
            MapBankSize +
            ControllerSize +
            CodeSize +
            StorageSize;

        using array_type = std::array<unsigned char, MemoryBytes>;
        using array_ptr = std::shared_ptr<array_type>;
        array_ptr _memory;
        BitArray<MemoryBytes> _bits;

        chaiscript::ChaiScript _scriptEngine;
        bool _mustQuit;
        
        using time_type = decltype(std::chrono::high_resolution_clock::now());
        
        time_type _exeStart;

        static std::shared_ptr<System> system;
    public:
    
        class ExitCalledException {};
    
        System() : _mustQuit{false}, _memory{std::make_shared<array_type>()}, _bits{_memory} { }

        chaiscript::ChaiScript & ScriptEngine() {
            return _scriptEngine;
        }

        void Update() {
            _scriptEngine.eval("update();");
        }
        
        bool HasUpdate() {
            auto update_it = _scriptEngine.get_locals().find("update");
            if(update_it != _scriptEngine.get_locals().end()) {
                try {
                    auto & update_func = update_it->second.get().cast<std::function<void()>>();
                } catch(...) {
                    return false;
                }
                return true;
            }
            return false;
        }

        bool MustQuit() {
            return _mustQuit;
        }

        bool LoadScript(std::string const& source) {
            nowide::cout << "Source size = " << source.length() << std::endl;
            assert((source.length() <= CodeSize) && "ERROR: Code is too big! Maximum of 256 KiB or 262144 Bytes.");
            //strncpy((char *)(_memory->data()), source.c_str(), CodeSize);
            strncpy_s((char *)(_memory->data()), CodeSize, source.c_str(), CodeSize);
            try {
                _scriptEngine.eval(source);
            } catch(ExitCalledException const& ex) {
                // Do nothing
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

        bool _btn(int id) {
            return false;
        }

        bool _btnp(int id, int hold = -1, int period = -1) {
            return false;
        }

        void _cls(int color = 0) {
            
        }

        void _exit() {
            _mustQuit = true;
            throw ExitCalledException();
        }

        int _pix(int x, int y, int color = -1) {
            return 0;
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

        static System & Get() {
            return *system;
        }

        static bool Btn(int id) {
            return system->_btn(id);
        }

        static bool Btnp(int id, int hold=-1, int period=-1) {
            return system->_btnp(id, hold, period);
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

        static void InitializeSystem() {
            using namespace chaiscript;

            system = std::make_shared<System>();

            auto & scriptEngine = system->ScriptEngine();

            //scriptEngine.add(fun(&System::Scanline), "scanline");
            scriptEngine.add(fun(&System::Btn), "btn");
            scriptEngine.add(fun(&System::Btnp), "btnp");
            scriptEngine.add(fun([](int id) -> bool { return System::Btnp(id); }), "btnp");
            scriptEngine.add(fun([](int id, int hold) -> bool { return System::Btnp(id, hold); }), "btnp");
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
        }

        static void UnitializeSystem() {
            system = nullptr;
        }
    };

}
