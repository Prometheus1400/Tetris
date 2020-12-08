#ifndef GRID_H
#define GRID_H
#include <chrono>
#include <thread>

#include "pieces.h"

extern const size_t BLOCK_SIZE;
extern const size_t WIDTH;
extern const size_t HEIGHT;
extern size_t       SCORE;

class GameGrid {
   private:
    // attributes
    size_t   GridWidth, GridHeight, BlockSize;
    Block*** _grid;
    // pointer to active piece, only one allowed at a time
    Piece* activePiece;

    // creates 2D array filled with nullptrs
    void initializeStructure();
    // sets each block to its corresponding grid position
    void mirrorBlocksToGrid() const;

    sf::RenderWindow* win;

   public:
    GameGrid() : GridWidth(WIDTH), GridHeight(HEIGHT), BlockSize(BLOCK_SIZE) { initializeStructure(); }
    ~GameGrid();

    void setWindow(sf::RenderWindow& window) { win = &window; }

    // deletes old piece and spawns a new one
    // note: piece virtual destructor should not delete the Block* inside of it since they will be
    // transfered to the GameGrid,
    void spawnNewPiece();

    // draws the grid and the piece
    void drawGrid() const;

    // transfers ownership of block* from piece to grid
    void movePieceToGrid();

    // returns true if specified coordinate is occupied by a Block object
    bool isBlock(int x, int y) const { return _grid[x][y]; }

    // checks the grid for any filled lines, returns y value of filled grid. else returns -1
    // checks from bottom up
    int checkForLine();
    // clears line found
    void clearLine(int yLine);
    // flashes line
    void flashLine(int yLine);
    // moves all blocks above cleared line down one
    void moveBlocksDown(int yLine);
    // removes all full lines
    void removeFullLines();

    // checks if there are any blocks in the piece's spawning area
    bool checkForGameOver();

    // all for moving / rotating the piece
    bool pieceCanMoveDown() const;
    bool pieceCanMoveRight() const;
    bool pieceCanMoveLeft() const;
    bool pieceCanRotate() const;
    void pieceDown();
    void pieceRight();
    void pieceLeft();
    void pieceRotate();
};

// PRIVATE
void GameGrid::initializeStructure() {
    _grid = new Block**[GridWidth];
    for (size_t i = 0; i < GridWidth; ++i) {
        _grid[i] = new Block*[GridHeight];
    }

    for (size_t i = 0; i < GridWidth; ++i) {
        for (size_t j = 0; j < GridHeight; ++j) {
            _grid[i][j] = nullptr;
        }
    }
}

// PUBLIC
GameGrid::~GameGrid() {
    if (activePiece) {
        delete activePiece;
    }

    for (size_t i = 0; i < GridWidth; ++i) {
        for (size_t j = 0; j < GridHeight; ++j) {
            if (_grid[i][j] != nullptr) {
                delete _grid[i][j];
            }
        }
    }
    for (size_t i = 0; i < GridWidth; ++i) {
        delete[] _grid[i];
    }
    delete[] _grid;
}

int GameGrid::checkForLine() {
    bool LineFull;
    for (size_t y = (HEIGHT - 1); y > 0; --y) {
        LineFull = true;
        for (size_t x = 0; x < WIDTH; ++x) {
            if (!isBlock(x, y)) {
                LineFull = false;
            }
        }
        if (LineFull) return y;
    }
    return -1;
}

bool GameGrid::checkForGameOver() {
    size_t xStart = (WIDTH / 2) - 2;
    size_t xEnd = (WIDTH / 2) + 2;

    for (size_t y = 0; y < 2; ++y) {
        for (size_t x = xStart; x < xEnd; ++x) {
            if (_grid[x][y]) return true;
        }
    }
    return false;
}

void GameGrid::flashLine(int yLine) {
    for (size_t x = 0; x < WIDTH; ++x) {
        _grid[x][yLine]->setColor(sf::Color::White);
    }
    win->clear(sf::Color(82, 86, 87, 56));
    drawGrid();
    win->display();
    std::this_thread::sleep_for(std::chrono::milliseconds(450));
    for (size_t x = 0; x < WIDTH; ++x) {
        _grid[x][yLine]->setColor(sf::Color::Transparent);
    }
    win->clear(sf::Color(82, 86, 87, 56));
    drawGrid();
    win->display();
}

void GameGrid::clearLine(int yLine) {
    flashLine(yLine);
    for (size_t x = 0; x < WIDTH; ++x) {
        if (_grid[x][yLine] != nullptr) {
            delete _grid[x][yLine];
            _grid[x][yLine] = nullptr;
            SCORE += 10;
        } else {
            throw std::invalid_argument("trying to delete not-full line");
        }
    }
}

void GameGrid::moveBlocksDown(int yLine) {
    for (size_t y = (size_t)(yLine - 1); y > 0; --y) {
        for (size_t x = 0; x < WIDTH; ++x) {
            if (_grid[x][y] != nullptr) {
                std::swap(_grid[x][y], _grid[x][y + 1]);
            }
        }
    }
}

void GameGrid::removeFullLines() {
    int yLine = checkForLine();
    // std::cout << "Line " << yLine << " is full" << std::endl;
    while (yLine != -1) {
        clearLine(yLine);
        moveBlocksDown(yLine);
        yLine = checkForLine();
    }
    mirrorBlocksToGrid();
    spawnNewPiece();
}

void GameGrid::mirrorBlocksToGrid() const {
    for (size_t i = 0; i < GridWidth; ++i) {
        for (size_t j = 0; j < GridHeight; ++j) {
            if (_grid[i][j] != nullptr) {
                _grid[i][j]->moveToGridPos(i, j);
            }
        }
    }
}

void GameGrid::movePieceToGrid() {
    Block*** structure = activePiece->getStructure();
    size_t   size = activePiece->getSize();
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (structure[i][j] != nullptr) {
                size_t x = structure[i][j]->GetXPos();
                size_t y = structure[i][j]->GetYPos();
                _grid[x][y] = structure[i][j];
            }
        }
    }
    activePiece->kill();
    delete activePiece;
    activePiece = nullptr;
    mirrorBlocksToGrid();
    // spawnNewPiece();
}

bool GameGrid::pieceCanMoveDown() const {
    Block*** structure = activePiece->getStructure();
    size_t   size = activePiece->getSize();

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (structure[i][j] != nullptr) {
                size_t x = structure[i][j]->GetXPos();
                size_t y = structure[i][j]->GetYPos();
                if (y >= (GridHeight - 1) || isBlock(x, y + 1)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool GameGrid::pieceCanMoveRight() const {
    Block*** structure = activePiece->getStructure();
    size_t   size = activePiece->getSize();

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (structure[i][j] != nullptr) {
                size_t x = structure[i][j]->GetXPos();
                size_t y = structure[i][j]->GetYPos();
                if (x >= (GridWidth - 1) || isBlock(x + 1, y)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool GameGrid::pieceCanMoveLeft() const {
    Block*** structure = activePiece->getStructure();
    size_t   size = activePiece->getSize();

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (structure[i][j] != nullptr) {
                size_t x = structure[i][j]->GetXPos();
                size_t y = structure[i][j]->GetYPos();
                if (x <= 0 || isBlock(x - 1, y)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool GameGrid::pieceCanRotate() const {
    Block***       structure = activePiece->getStructure();
    size_t         size = activePiece->getSize();
    unsigned short rotStage = activePiece->getRotationStage();

    size_t xPositions[size * size];
    size_t yPositions[size * size];
    size_t arrSize = 0;

    for (size_t x = 0; x < size; ++x) {
        for (size_t y = 0; y < size; ++y) {
            if (structure[x][y] != nullptr) {
                xPositions[arrSize] = x;
                yPositions[arrSize] = y;
                ++arrSize;
            }
        }
    }
    switch (rotStage) {
        case 1:
        case 3:
            for (size_t i = 0; i < arrSize; ++i) {
                size_t x = xPositions[i];
                size_t y = yPositions[i];
                // std::cout << activePiece->getAbsGridX(y) << " " << activePiece->getAbsGridY(x) << std::endl;
                if (activePiece->getAbsGridX(y) >= (WIDTH) || activePiece->getAbsGridX(y) < 0 || activePiece->getAbsGridY(x) >= (HEIGHT - 1) ||
                    activePiece->getAbsGridY(x) < 0 || isBlock(activePiece->getAbsGridX(y), activePiece->getAbsGridY(x)))
                    return false;
            }
            break;
        case 2:
        case 4:
            for (size_t i = 0; i < arrSize; ++i) {
                size_t x = xPositions[i];
                size_t y = yPositions[i];
                if (activePiece->getAbsGridX(size - 1 - y) >= (WIDTH) || activePiece->getAbsGridX(size - 1 - y) < 0 ||
                    activePiece->getAbsGridY(size - 1 - x) >= (HEIGHT - 1) || activePiece->getAbsGridY(size - 1 - x) < 0 ||
                    isBlock(activePiece->getAbsGridX(size - 1 - y), activePiece->getAbsGridY(size - 1 - x)))
                    return false;
            }
            break;
    }
    return true;
}

void GameGrid::pieceDown() {
    if (pieceCanMoveDown()) {
        activePiece->down();
    } else {
        movePieceToGrid();
        removeFullLines();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void GameGrid::pieceRight() {
    if (pieceCanMoveRight()) {
        activePiece->right();
    }
}

void GameGrid::pieceLeft() {
    if (pieceCanMoveLeft()) {
        activePiece->left();
    }
}

void GameGrid::pieceRotate() {
    if (pieceCanRotate()) {
        activePiece->Rotate();
    }
}

void GameGrid::spawnNewPiece() {
    if (activePiece != nullptr) {
        delete activePiece;
    }
    size_t num = rand() % 5;
    switch (num) {
        case 0:
            if (rand() % 2 == 0)
                activePiece = new Piece_1(GridWidth / 2, 0);
            else
                activePiece = new Piece_1R(GridWidth / 2, 0);
            break;
        case 1:
            activePiece = new Piece_2(GridWidth / 2, 0);
            break;
        case 2:
            activePiece = new Piece_3(GridWidth / 2, 0);
            break;
        case 3:
            activePiece = new Piece_4(GridWidth / 2, 0);
            break;
        case 4:
            if (rand() % 2 == 0)
                activePiece = new Piece_5(GridWidth / 2, 0);
            else
                activePiece = new Piece_5R(GridWidth / 2, 0);
            break;
    }
}

void GameGrid::drawGrid() const {
    if (activePiece != nullptr) {
        activePiece->drawPiece(*win);
    }

    for (size_t i = 0; i < WIDTH; ++i) {
        for (size_t j = 0; j < HEIGHT; ++j) {
            if (_grid[i][j] != nullptr) {
                _grid[i][j]->drawBlock(*win);
            }
        }
    }
}

#endif