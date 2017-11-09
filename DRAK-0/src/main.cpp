#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <chaiscript/chaiscript.hpp>

double function(int i, double j) {
    return i * j;
}

int main(int argc, char * argv[]) {
    chaiscript::ChaiScript chai;
    chai.add(chaiscript::fun(&function), "function");
    double d = chai.eval<double>("function(3, 4.75);");
    std::cout << "function(3, 4.75) ==> " << d << std::endl;

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
