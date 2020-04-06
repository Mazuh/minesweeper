#ifndef MINEFIELD_CPP
#define MINEFIELD_CPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <tuple>
#include "constants.cpp"

class MineField {
public:
    MineField(int width, int height, int bombsQtt) {
        this->_width = width;
        this->_height = height;
        this->_bombsQtt = bombsQtt;

        this->_generateUnknowns();
        this->_generateBombs();
    }
    bool isAllBombsFlagged() {
        if (this->getUsedBombFlagsQtt() != this->getBombsQtt()) {
            return false;
        }

        for (auto bomb : this->_bombs) {
            int x, y;
            std::tie (x, y) = bomb;
            if (this->getDisplay(x, y) != BOMB_FLAG_STR) {
                return false;
            }
        }

        return true;
    }
    bool isValidPosition(int x, int y) {
        return x >= 0 && y >= 0 && x < this->_width && y < this->_height;
    }
    bool isBombTouched() {
        return this->_isBombTouched;
    }
    std::string getDisplay(int x, int y) {
        return this->_cols[y][x];
    }
    bool isRevealed(int x, int y) {
        std::string value = this->getDisplay(x, y);
        return value != UNKNOWN_STR
            && value != QUESTION_BOMB_FLAG_STR
            && value != BOMB_FLAG_STR;
    }
    bool isAllowedToFlag(int x, int y) {
        return this->getDisplay(x, y) == UNKNOWN_STR;
    }
    int getUsedBombFlagsQtt() {
        return this->_usedBombFlagsQtt;
    }
    int getBombsQtt() {
        return this->_bombsQtt;
    }
    bool setEmptyFlag(int x, int y) {
        std::string value = this->getDisplay(x, y);
        if (this->isRevealed(x, y)) {
            return false;
        }

        if (value == BOMB_FLAG_STR) {
            this->_usedBombFlagsQtt--;
        }

        this->_setDisplay(x, y, UNKNOWN_STR);
        return true;
    }
    bool addQuestionFlag(int x, int y) {
        if (!this->isValidPosition(x, y) || this->isRevealed(x, y)) {
            return false;
        }

        this->setEmptyFlag(x, y);
        this->_setDisplay(x, y, QUESTION_BOMB_FLAG_STR);
        return true;
    }
    bool addBombFlag(int x, int y) {
        if (!this->isValidPosition(x, y) || this->isRevealed(x, y)) {
            return false;
        }

        this->setEmptyFlag(x, y);
        this->_setDisplay(x, y, BOMB_FLAG_STR);
        this->_usedBombFlagsQtt++;
        return true;
    }
    bool touch(int x, int y) {
        if (!this->isValidPosition(x, y)) {
            return false;
        }

        bool isBomb = this->_isBomb(x, y);

        if (isBomb) {
            this->_setDisplay(x, y, EXPLODING_BOMB_STR);
        } else {
            this->_handleSafeTouch(x, y);
        }

        this->_isBombTouched = isBomb;
        return true;
    }
    void revealAll() {
        for (int y = 0; y < this->_height; y++) {
            for (int x = 0; x < this->_width; x++) {
                if (this->_isBomb(x, y) && this->getDisplay(x, y) != EXPLODING_BOMB_STR) {
                    this->_setDisplay(x, y, BOMB_STR);
                } else if (!this->isRevealed(x, y)) {
                    this->_handleSafeTouch(x, y);
                }
            }
        }
    }
    std::string toString() {
        std::stringstream stringified;

        stringified << BLANK_OFFSET_STR << BLANK_OFFSET_STR;
        for (int i = 0; i < this->_width; i++) {
            stringified << numberToDisplay(i);
        }
        stringified << std::endl;

        stringified << BLANK_OFFSET_STR << FIELD_LEFT_VERTICAL_WALL_STR;
        for (int i = 1; i < this->_width + 2; i++) {
            stringified << FIELD_HORIZONTAL_WALL_STR;
        }
        stringified << std::endl;

        for (int i = 0; i < this->_height; i++) {
            stringified << numberToDisplay(i) << FIELD_LEFT_VERTICAL_WALL_STR;

            auto cells = this->_cols[i];
            for (auto cell : cells) {
                stringified << cell;
            }
            stringified << FIELD_VERTICAL_WALL_STR << std::endl;
        }

        stringified << BLANK_OFFSET_STR << FIELD_LEFT_VERTICAL_WALL_STR;
        for (int i = 1; i < this->_width + 2; i++) {
            stringified << FIELD_HORIZONTAL_WALL_STR;
        }
        stringified << std::endl;

        return stringified.str();
    }
private:
    int _width;
    int _height;
    int _bombsQtt;
    int _usedBombFlagsQtt = 0;
    bool _isBombTouched = false;
    std::vector<std::vector<std::string>> _cols;
    std::vector<std::tuple<int, int>> _bombs;
    void _generateUnknowns() {
        for (int y = 0; y < this->_height; y++) {
            std::vector<std::string> cells;
            for (int x = 0; x < this->_width; x++) {
                cells.push_back(UNKNOWN_STR);
            }
            this->_cols.push_back(cells);
        }
    }
    void _generateBombs() {
        while ((int) this->_bombs.size() < this->_bombsQtt) {
            int randomX = rand() % this->_width;
            int randomY = rand() % this->_height;
            if (!this->_isBomb(randomX, randomY)) {
                this->_bombs.push_back(std::make_tuple(randomX, randomY));
            }
        }
    }
    void _setDisplay(int x, int y, std::string value) {
        this->_cols[y][x] = value;
    }
    bool _isBomb(int x, int y) {
        if (!this->isValidPosition(x, y)) {
            return false;
        }

        for (auto bomb : this->_bombs) {
            int bombX, bombY;
            std::tie (bombX, bombY) = bomb;
            if (x == bombX && y == bombY) {
                return true;
            }
        }

        return false;
    }
    void _handleSafeTouch(int x, int y) {
        this->setEmptyFlag(x, y);

        int bombsAroundQtt = 0;

        std::array<std::tuple<int, int>, 8> surroundings = {
            std::make_tuple(x - 1, y - 1),
            std::make_tuple(x - 1, y),
            std::make_tuple(x - 1, y + 1),
            std::make_tuple(x + 1, y - 1),
            std::make_tuple(x + 1, y),
            std::make_tuple(x + 1, y + 1),
            std::make_tuple(x, y - 1),
            std::make_tuple(x, y + 1),
        };

        this->_setDisplay(x, y, BLANK_STR);

        for (auto surrounding : surroundings) {
            int surroundingX, surroundingY;
            std::tie (surroundingX, surroundingY) = surrounding;
            if (this->_isBomb(surroundingX, surroundingY)) {
                bombsAroundQtt++;
                this->_setDisplay(x, y, numberToDisplay(bombsAroundQtt));
            }
        }

        if (bombsAroundQtt) {
            return;
        }

        for (auto surrounding : surroundings) {
            int surroundingX, surroundingY;
            std::tie (surroundingX, surroundingY) = surrounding;
            if (this->isValidPosition(surroundingX, surroundingY)
                && !this->isRevealed(surroundingX, surroundingY)
                && !this->_isBomb(surroundingX, surroundingY)) {
                this->_handleSafeTouch(surroundingX, surroundingY);
            }
        }
    }
    static std::string numberToDisplay(int n) {
        return " " + std::to_string(n) + (n < 10 ? " " : "");
    }
};

#endif
