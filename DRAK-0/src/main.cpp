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

int main(int argc, char * argv[]) {
    std::string do_source;
    if(argc >= 1) {
        std::string filename = argv[1];
        do_source = readFile(filename);
    } else {
        do_source = source;
    }

    drak::System::InitializeSystem();

    try {

        auto & sys = drak::System::Get();

        sf::RenderWindow window(sf::VideoMode(800, 600), "DRAK-0");

        // Load Script/Cartridge
        if(!sys.LoadScript(do_source)) {
            nowide::cerr << "ERROR: Missing `update` function\n";
        }

        while(window.isOpen() && !sys.MustQuit()) {
            sf::Event event;

            while(window.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    window.close();
                }
            }
            
            try {
                sys.Update();
            } catch(drak::System::ExitCalledException const& ex) {
                // Do nothing
            }

            window.clear();
            window.display();
        }

    } catch(...) {
        drak::System::UnitializeSystem();
    }

    drak::System::UnitializeSystem();

    return 0;
}
