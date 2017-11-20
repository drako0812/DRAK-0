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
#include "pch.h"

#include "system.hpp"

std::shared_ptr<drak::System> drak::System::system = nullptr;

constexpr char source[] = R"(
trace("Starting Up...");

def update() {
  // Do things here
}

trace("Shutting Down...");
exit()
)";

std::string readFile(std::string const& filename) {
    nowide::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
    if(in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    throw errno;
}

sf::View calcView(sf::Vector2u const& win_size, sf::Vector2u const& desired_size) {
    sf::FloatRect viewport(0, 0, 1.0f, 1.0f);
    float scrn_width = win_size.x / static_cast<float>(desired_size.x);
    float scrn_height = win_size.y / static_cast<float>(desired_size.y);
    if(scrn_width > scrn_height) {
        viewport.width = scrn_height / scrn_width;
        viewport.left = (1.0f - viewport.width) / 2.0f;
    } else if(scrn_width < scrn_height) {
        viewport.height = scrn_width / scrn_height;
        viewport.top = (1.0f - viewport.height) / 2.0f;
    }
    sf::View view(sf::FloatRect(0.0f, 0.0f, desired_size.x, desired_size.y));
    view.setViewport(viewport);
    return view;
}

const sf::Vector2u desired_size(320, 240);

int main(int argc, char * argv[]) {
    std::string do_source;
    drak::System::ScriptLanguage lang;
    if(argc >= 2) {
        std::string lang_str = argv[1];
        if(lang_str == "chai") {
            lang = drak::System::ScriptLanguage::ChaiScript;
        } else if(lang_str == "lua") {
            lang = drak::System::ScriptLanguage::Lua;
        } else {
            nowide::cout << "ERROR: Invalid language identifier" << std::endl;
            nowide::cout << "USAGE:" << std::endl;
            nowide::cout << "DRAK-0.exe chai <chaiscript_file.chai>" << std::endl;
            nowide::cout << "DRAK-0.exe lua <lua_file.lua>" << std::endl;
            return 0;
        }
        std::string filename = argv[2];
        do_source = readFile(filename);

    } else {
        //do_source = source;
        nowide::cout << "ERROR: No input script" << std::endl;
        nowide::cout << "USAGE:" << std::endl;
        nowide::cout << "DRAK-0.exe chai <chaiscript_file.chai>" << std::endl;
        nowide::cout << "DRAK-0.exe lua <lua_file.lua>" << std::endl;
        return 0;
    }

    drak::System::InitializeSystem();

    try {

        auto & sys = drak::System::Get();
        auto & screen = sys.Screen();

        sf::RenderWindow window(sf::VideoMode(800, 600), "DRAK-0");
        window.setFramerateLimit(60);
        window.setMouseCursorVisible(false);
        window.setView(calcView(window.getSize(), desired_size));

        sf::RenderTexture render_tex;
        render_tex.create(320, 240);
        sf::Sprite render_sprite(render_tex.getTexture());
        render_sprite.setOrigin(320 / 2.0f, 240 / 2.0f);
        render_sprite.setPosition(320 / 2.0f, 240 / 2.0f);

        // Load Script/Cartridge
        if(!sys.LoadScript(lang, do_source)) {
            nowide::cout << "ERROR: Missing `update` function" << std::endl;
            sys.Quit();
        }

        while(window.isOpen() && !sys.MustQuit()) {
            sf::Event event;

            while(window.pollEvent(event)) {
                switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                }
            }
            
            try {
                sys.Update(window, calcView(window.getSize(), desired_size));
                //nowide::cout << "UPDATE" << std::endl;
                //screen.Draw();
            } catch(drak::System::ExitCalledException const& ex) {
                // Do nothing
            }

            screen.DrawOnScreen(render_tex);
            render_tex.display();

            window.setView(calcView(window.getSize(), desired_size));
            window.clear();
            window.draw(render_sprite);
            window.display();
        }

    } catch(...) {
        drak::System::UnitializeSystem();
        throw;
    }

    drak::System::UnitializeSystem();

    nowide::clog.flush();
    nowide::cerr.flush();
    nowide::cout.flush();

    return 0;
}
