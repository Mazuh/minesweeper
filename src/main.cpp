#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>

const std::string BOMB_STR = "💣";

const std::string UNKNOWN_STR = "❓";

const std::string BLANK_STR = "⬜";

const std::string FIELD_HORIZONTAL_WALL_STR = "🧱";

const std::string FIELD_VERTICAL_WALL_STR = "🧱";

class Field {
private:
    int _width;
    int _height;
    int _bombsQtt;
    bool _isBombTouched = false;
    std::vector<std::vector<std::string>> rows;
    std::vector<std::tuple<int, int>> bombs;
    void _fillEmpty() {
        for (int iRows = 0; iRows < _width; ++iRows) {
            std::vector<std::string> cols;
            for (int iCols = 0; iCols < _height; ++iCols) {
                cols.push_back(UNKNOWN_STR);
            }
            this->rows.push_back(cols);
        }
    }
    void _generateBombs() {
        while ((int) this->bombs.size() < this->_bombsQtt) {
            int randomX = rand() % this->_width;
            int randomY = rand() % this->_height;
            if (!this->_isBombAt(randomX, randomY)) {
                this->bombs.push_back(std::make_tuple(randomX, randomY));
            }
        }
    }
    bool _isBombAt(int x, int y) {
        for (auto bomb : this->bombs) {
            int bombX, bombY;
            std::tie (bombX, bombY) = bomb;
            if (x == bombX && y == bombY) {
                return true;
            }
        }

        return false;
    }
public:
    Field(int width, int height, int bombsQtt) {
        this->_width = width;
        this->_height = height;
        this->_bombsQtt = bombsQtt;

        this->_fillEmpty();
        this->_generateBombs();
    }
    void touchAt(int x, int y) {
        bool isBomb = this->_isBombAt(x, y);

        if (isBomb) {
            this->rows[x][y] = BOMB_STR;
        } else {
            this->rows[x][y] = BLANK_STR;
        }

        this->_isBombTouched = isBomb;
    }
    bool isBombTouched() {
        return this->_isBombTouched;
    }
    std::string toString() {
        std::stringstream stringified;

        for (int i = 0; i < this->_width + 2; i++) {
            stringified << FIELD_HORIZONTAL_WALL_STR;
        }

        stringified << std::endl;

        for (auto cols : this->rows) {
            stringified << FIELD_VERTICAL_WALL_STR;
            for (auto cell : cols) {
                stringified << cell;
            }
            stringified << FIELD_VERTICAL_WALL_STR << std::endl;
        }

        for (int i = 0; i < this->_width + 2; i++) {
            stringified << FIELD_HORIZONTAL_WALL_STR;
        }

        stringified << std::endl;

        return stringified.str();
    }
};

int main() {
    int x, y;
    Field field (8, 8, 10);

    while (true) {
        std::system("clear || cls");

        std::cout
            << std::endl
            << "🕵 Minesweeper (by Mazuh)"
            << std::endl
            << std::endl
            << field.toString()
            << std::endl;

        if (field.isBombTouched()) {
            std::cout << "🔴 Ops! Triggered a bomb!" << std::endl;
            break;
        }

        std::cout << "Select X and Y (separated by spaces): ";
        std::cin.clear();
        std::cin >> x >> y;
        field.touchAt(x, y);
    }

    return 0;
}
