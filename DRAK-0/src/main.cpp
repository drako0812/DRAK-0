#include "pch.h"

#include "system.hpp"

double function(int i, double j) {
    return i * j;
}


int main(int argc, char * argv[]) {
    drak::System system;

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
