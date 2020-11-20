#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "src/grid.h"

const size_t BLOCK_SIZE = 50;  // size in pixels
const size_t WIDTH  = 15;      // size in blocks/grid sections
const size_t HEIGHT = 20;      // size in blocks/grid sections
size_t SCORE  = 0;

int main() {
	srand(time(0));
	int realWidth  = (int) BLOCK_SIZE * WIDTH;
	int realHeight = (int) BLOCK_SIZE * HEIGHT;

	sf::RenderWindow window(sf::VideoMode(realWidth, realHeight), "Tetris");
	window.setKeyRepeatEnabled(false);

	GameGrid Grid; //Width, height, and block size
	Grid.setWindow(window);
	Grid.spawnNewPiece();

	sf::Clock clock;
	sf::Time time;
	while (window.isOpen()) {
       // event management
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
	  		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			    Grid.pieceRotate();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			    Grid.pieceDown();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			    Grid.pieceRight();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			    Grid.pieceLeft();
			}
        }

        time = clock.getElapsedTime();
        if (time.asSeconds() > 0.5) {
        	Grid.pieceDown();
        	clock.restart();
        }

        if (Grid.checkForGameOver())
        	window.close();
        window.clear(sf::Color(82, 86, 87, 56));
        Grid.drawGrid();
        window.display();
    }
    std::cout << "Game Over with a Score of: " << SCORE << std::endl;
    return 0;
}
