#include "pch.h"

double function(int i, double j) {
    return i * j;
}

constexpr char code[] = 
R"(
print(function(3, 4.75));
)";

int main(int argc, char * argv[]) {
    chaiscript::ChaiScript chai;
    chai.add(chaiscript::fun(&function), "function");
    chai(code);

    sf::RenderWindow window(sf::VideoMode(800, 600), "DRAK-0");

    while(window.isOpen()) {
        sf::Event event;

        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.display();
    }

    return 0;
}
