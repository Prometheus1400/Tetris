#include "piece.h"

// Derived Test Class
class Piece_1 : public Piece { // Z piece
protected:
	void setStructure() override {
		setColor();
		// first index is x pos, second index is y pos
		_structure[0][0] = new Block(_color);
		_structure[1][0] = new Block(_color);
		_structure[1][1] = new Block(_color);
		_structure[2][1] = new Block(_color);
		mirrorStructureToBlocks();
	}
public:
	Piece_1(size_t x, size_t y) : Piece(3,x,y) { setStructure(); }
};

class Piece_2 : public Piece { // Half Plus looking piece
protected:
	void setStructure() override {
		setColor();
		// first index is x pos, second index is y pos
		_structure[0][0] = new Block(_color);
		_structure[1][0] = new Block(_color);
		_structure[2][0] = new Block(_color);
		_structure[1][1] = new Block(_color);
		mirrorStructureToBlocks();
	}
public:
	Piece_2(size_t x, size_t y) : Piece(3,x,y) { setStructure(); }
};

class Piece_3 : public Piece { //Square piece
protected:
	void setStructure() override {
		setColor();
		// first index is x pos, second index is y pos
		_structure[0][0] = new Block(_color);
		_structure[1][0] = new Block(_color);
		_structure[0][1] = new Block(_color);
		_structure[1][1] = new Block(_color);
		mirrorStructureToBlocks();
	}
public:
	Piece_3(size_t x, size_t y) : Piece(3,x,y) { setStructure(); }
};

class Piece_4 : public Piece { //Straight Line
protected:
	void setStructure() override {
		setColor();
		// first index is x pos, second index is y pos
		_structure[0][0] = new Block(_color);
		_structure[1][0] = new Block(_color);
		_structure[2][0] = new Block(_color);
		_structure[3][0] = new Block(_color);
		mirrorStructureToBlocks();
	}
public:
	Piece_4(size_t x, size_t y) : Piece(4,x,y) { setStructure(); }
};

class Piece_5 : public Piece { //L looking piece
protected:
	void setStructure() override {
		setColor();
		// first index is x pos, second index is y pos
		_structure[0][1] = new Block(_color);
		_structure[1][1] = new Block(_color);
		_structure[2][1] = new Block(_color);
		_structure[3][1] = new Block(_color);
		_structure[0][2] = new Block(_color);
		mirrorStructureToBlocks();
	}
public:
	Piece_5(size_t x, size_t y) : Piece(4,x,y) { setStructure(); }
};