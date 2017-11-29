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

    System::System() : _mustQuit{false}, _memory{std::make_shared<array_type>()} {
        _lang = ScriptLanguage::ChaiScript;
        _bits = BitArray<MemoryBytes>(gsl::span<unsigned char>{*_memory});
        auto * _screen_ptr = _memory->data();
        auto * _palette_ptr = _memory->data() + PaletteOffset;
        auto * _sprite_ptr = _memory->data() + SpriteBankOffset;
        auto _screen_span = gsl::span<unsigned char>{_screen_ptr, static_cast<std::size_t>(ScreenSize)};
        auto _palette_span = gsl::span<unsigned char>{_palette_ptr, static_cast<std::size_t>(PaletteSize)};
        auto _sprite_span = gsl::span<unsigned char>{_sprite_ptr, static_cast<std::size_t>(SpriteBankSize)};
        _screen = drak::Screen(_screen_span, _palette_span, _sprite_span);
        _screen.SetFullPalette(DefaultPalette);
        auto * _input_ptr = _memory->data() + ControllerOffset;
        auto _input_span = gsl::span<unsigned char>{_input_ptr, static_cast<std::size_t>(ControllerSize)};
        _input = drak::Input(_input_span);
    }

    void System::SetLanguage(ScriptLanguage lang) {
        _lang = lang;
        _memory->at(LanguageOffset) = static_cast<unsigned char>(lang);
    }

    System::ScriptLanguage System::GetLanguage() const {
        return _lang;
    }

    chaiscript::ChaiScript & System::ChaiScriptEngine() {
        return _chai_scriptEngine;
    }

    sol::state & System::LuaScriptEngine() {
        return _lua_scriptEngine;
    }

    Screen & System::Screen() {
        return _screen;
    }

    void System::Update(sf::RenderWindow & window, sf::View const& view) {
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

    bool System::HasUpdate() {
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

    bool System::MustQuit() {
        return _mustQuit;
    }

    void System::Quit() {
        _mustQuit = true;
    }

    bool System::LoadScript(ScriptLanguage lang, std::string const& source) {
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

    bool System::ImportSprites(std::string const& source) {
        sf::Image img;
        if(!img.loadFromFile(source)) {
            nowide::cout << "ERROR: Failed to load source image \"" << source << "\"" << std::endl;
            return false;
        }

        if((img.getSize().x != SpriteBankDim) && (img.getSize().y != SpriteBankDim * 5)) {
            nowide::cout << "ERROR: source image is of incorrect size X=" << img.getSize().x << ", Y=" << img.getSize().y << " - Expected X=" << SpriteBankDim << ", Y=" << SpriteBankDim * 5 << std::endl;
            return false;
        }

        // We try to match the RGB values in the image (this only works with default palette right now)
        for(int y = 0; y < SpriteBankDim * 5; y++) {
            for(int x = 0; x < SpriteBankDim; x++) {
                auto c = img.getPixel(x, y);
                _memory->at(SpriteBankOffset + (x + y * SpriteBankDim)) = static_cast<unsigned char>(_rgb(c.r, c.g, c.b, 1));
            }
        }

        return true;
    }

}
