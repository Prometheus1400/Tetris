#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <string>
#include "piece.h"

// size of each block in pixels. declared above main.cpp
extern const size_t BLOCK_SIZE;

class Block : public sf::Sprite {
protected:
	// absolute x and y pos in grid coords
	size_t x_pos, y_pos;
	// texture loaded and set in the constructor
	sf::Texture blockTexture;

public:
	// constructs block with x and y grid position
	Block(std::string color);

	void SetColor(std::string color);

	// return x and y positions
	size_t GetXPos() const { return x_pos; }
	size_t GetYPos() const { return y_pos; }

	// moves the block to the absolute grid position
	// updates x_pos, and y_pos and moves the Block
	void moveToGridPos(size_t x, size_t y);
	void down();
	void right();
	void left();

	// draws the object, called from Piece classes
	void drawBlock(sf::RenderWindow& win);
};

void Block::SetColor(std::string color) {
	if (color == "red") {
		this->setColor(sf::Color::Red);
	} else if (color == "blue") {
		this->setColor(sf::Color::Blue);
	} else if (color == "magenta") {
		this->setColor(sf::Color::Magenta);
	} else if (color == "green") {
		this->setColor(sf::Color::Green);
	} else if (color == "yellow") {
		this->setColor(sf::Color::Yellow);
	} else if (color == "white") {
		this->setColor(sf::Color::White);
	} else if (color == "black") {
		this->setColor(sf::Color::Black);
	} else {
		throw std::invalid_argument("invalid color argument");
	}
}

Block::Block(std::string color) {
	if (!blockTexture.loadFromFile("icons/block.png")) { 
		throw std::invalid_argument("icon cant be opened"); 
	}
	SetColor(color);
	// for the scale. 118 pixels is size of original image
	float BlockSize = BLOCK_SIZE / 118.f;
	this->setTexture(blockTexture);
	this->setScale(BlockSize, BlockSize);
}

void Block::drawBlock(sf::RenderWindow& win) {
	win.draw(*this);
}

void Block::moveToGridPos(size_t x, size_t y) {
	x_pos = x;
	y_pos = y;
	this->setPosition(BLOCK_SIZE*x, BLOCK_SIZE*y);
}

void Block::down() {}






// For Debugging
std::ostream& operator<<(std::ostream& os, Block& block) {
	os << block.GetXPos() << "   " << block.GetYPos();

	return os;
}








#endif