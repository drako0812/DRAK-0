#pragma once

#include "bit_array.hpp"

namespace drak {

    class System {
        BitArray<10 * 1024 * 1024> _memory;

        chaiscript::ChaiScript _scriptEngine;
        bool _mustQuit;
    public:
        System() : _mustQuit{false} { }

        void Initialize() { }

        void Update() {
            auto update_it = _scriptEngine.get_locals().find("update");
            if(update_it != _scriptEngine.get_locals().end()) {
                update_it->second.get().cast<std::function<void()>>()();
            } else {
                nowide::cerr << "ERROR: Cartridge must have a function \"update\" defined!\n";
                _mustQuit = true;
            }
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
        // music
        // peek
        // peek4
        // pix
        // TODO: Finish
    };

}