#ifndef PIECE_H
#define PIECE_H
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <string>
#include "block.h"

extern const size_t BLOCK_SIZE;
extern const size_t WIDTH;       // size in blocks/grid sections
extern const size_t HEIGHT;      // size in blocks/grid sections

// abstract class for pieces
class Piece {
protected:
	// color of piece, red - blue - magenta - green - yellow
	std::string _color;
	// information for destructor
	bool killed = false;
	// 2D array containing Block* or nullptrs
	Block*** _structure;
	size_t _size;
	// the absolute y position of the Piece object
	size_t _absYPos, _absXPos;
	// the current rotation stage 1->2->3->4->1...
	unsigned short _rotationStage;

	// indicates if the class is still "alive"
	// or if the Block*s belong to GameGrid now
	bool transferedToGrid;

	// sets up the 2D array of pointers, sets all of them to nullptr
	void initializeStructure();
	// each subclass has a unique structure
	virtual void setStructure() = 0;
	// reflects block position in _structure to the actual position
	void mirrorStructureToBlocks() const;
	// sets the color randomly
	void setColor();

public:
	// constructs piece with size of structure, intended to be 3x3 or 4x4
	// subclasses should call this in constructor with desired size, calls initializeStructure()
	Piece(size_t size, size_t xPos, size_t yPos);
	// subclasses do not need to override the base destructor
	virtual ~Piece();

	//grabs the absolute coordinates from local grid
	size_t getAbsGridX(size_t x) const { return (x + _absXPos); }
	size_t getAbsGridY(size_t y) const { return (y + _absYPos); }
	const unsigned short getRotationStage() const { return _rotationStage; }

	//kills object
	void kill() { killed = true; }

	// rotates the piece counterclockwise
	void Rotate();
	// used for collision detection in grid class
	Block*** getStructure() const { return _structure; }
	size_t getSize() const { return _size; }

	// moves the piece
	void down();
	void left();
	void right();
	// draws each individual block in the piece
	void drawPiece(sf::RenderWindow& win) const;

};

Piece::Piece(size_t size, size_t xPos, size_t yPos)
	: _size(size), transferedToGrid(false), _rotationStage(1), _absXPos(xPos), _absYPos(yPos) {
	initializeStructure();
}

Piece::~Piece()  {
	for (size_t i = 0; i < _size; ++i) {
		for (size_t j = 0; j < _size; ++j) {
			if (_structure[i][j] != nullptr && !killed) {
				delete _structure[i][j];
			}
		}
	}
	for (size_t i = 0; i < _size; ++i) {
		delete[] _structure[i];
	}
	delete[] _structure;
}

void Piece::initializeStructure() {
	_structure = new Block**[_size];
	for (size_t i = 0; i < _size; ++i) {
		_structure[i] = new Block*[_size];
	}

	for (size_t i = 0; i < _size; ++i) {
		for (size_t j = 0; j < _size; ++j) {
			_structure[i][j] = nullptr;
		}
	}
}

void Piece::mirrorStructureToBlocks() const {
	for (size_t i = 0; i < _size; ++i) {
		for (size_t j = 0; j < _size; ++j) {
			if (_structure[i][j] != nullptr) {
				_structure[i][j]->moveToGridPos(i + _absXPos, j + _absYPos);
			}
		}
	}
}

void Piece::Rotate() {
	size_t xPositions[_size*_size];
	size_t yPositions[_size*_size];
	size_t arrSize = 0;

	for (size_t x = 0; x < _size; ++x) {
		for (size_t y = 0; y < _size; ++y) {
			if (_structure[x][y] != nullptr) {
				xPositions[arrSize] = x;
				yPositions[arrSize] = y;
				++arrSize;
			}
		}
	}
	size_t x = -1;
	size_t y = -1;
	switch (_rotationStage) {
		case 1:
			for (size_t i = 0; i < arrSize; ++i) {
				x = xPositions[i];
				y = yPositions[i];
				std::swap(_structure[x][y],_structure[y][x]);
			}
			_rotationStage = 2;
			break;
		case 2:
			for (size_t i = 0; i < arrSize; ++i) {
				x = xPositions[i];
				y = yPositions[i];
				std::swap(_structure[x][y],_structure[_size-1-y][_size-1-x]);
			}
			_rotationStage = 3;
			break;
		case 3:
			for (size_t i = 0; i < arrSize; ++i) {
				x = xPositions[i];
				y = yPositions[i];
				std::swap(_structure[x][y],_structure[y][x]);
			}
			_rotationStage = 4;
			break;
		case 4:
			for (size_t i = 0; i < arrSize; ++i) {
				x = xPositions[i];
				y = yPositions[i];
				std::swap(_structure[x][y],_structure[_size-1-y][_size-1-x]);
			}
			_rotationStage = 1;
			break;
		default:
			throw std::invalid_argument("rotationstage in impossible stage!");
	}

	mirrorStructureToBlocks();
}

void Piece::down() {
	_absYPos++;
	mirrorStructureToBlocks();
}
void Piece::right() {
	_absXPos++;
	mirrorStructureToBlocks();
}
void Piece::left() {
	_absXPos--;
	mirrorStructureToBlocks();
}

void Piece::drawPiece(sf::RenderWindow& win) const {
	for (size_t i = 0; i < _size; ++i) {
		for (size_t j = 0; j < _size; ++j) {
			if (_structure[i][j] != nullptr) {
				_structure[i][j]->drawBlock(win);
			}
		}
	}
}

void Piece::setColor() {
	size_t num = rand() % 5;
	switch (num) {
		case 0:
			_color = "red";
			break;
		case 1:
			_color = "blue";
			break;
		case 2:
			_color = "magenta";
			break;
		case 3:
			_color = "green";
			break;
		case 4:
			_color = "yellow";
			break;
	}
}
#endif