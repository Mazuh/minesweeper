#include <cstdlib>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <tuple>

const std::string BOMB_STR = "💣";

const std::string UNKNOWN_STR = "❓";

const std::string BLANK_STR = " -";

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
        if (!this->isValidPosition(x, y)) {
            return false;
        }

        for (auto bomb : this->bombs) {
            int bombX, bombY;
            std::tie (bombX, bombY) = bomb;
            if (x == bombX && y == bombY) {
                return true;
            }
        }

        return false;
    }
    void _handleSafeTouch(int x, int y) {
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

        this->rows[x][y] = BLANK_STR;

        for (auto surrounding : surroundings) {
            int surroundingX, surroundingY;
            std::tie (surroundingX, surroundingY) = surrounding;
            if (this->_isBombAt(surroundingX, surroundingY)) {
                bombsAroundQtt++;
                this->rows[x][y] = " " + std::to_string(bombsAroundQtt);
            }
        }

        if (bombsAroundQtt) {
            return;
        }

        for (auto surrounding : surroundings) {
            int surroundingX, surroundingY;
            std::tie (surroundingX, surroundingY) = surrounding;
            if (this->isValidPosition(surroundingX, surroundingY)
                && !this->_isBombAt(surroundingX, surroundingY)
                && this->rows[surroundingX][surroundingY] == UNKNOWN_STR) {
                this->_handleSafeTouch(surroundingX, surroundingY);
            }
        }
    }
public:
    Field(int width, int height, int bombsQtt) {
        this->_width = width;
        this->_height = height;
        this->_bombsQtt = bombsQtt;

        this->_fillEmpty();
        this->_generateBombs();
    }
    bool isValidPosition(int x, int y) {
        return x >= 0 && y >= 0 && x < this->_width && y < this->_height;
    }
    void touchAt(int x, int y) {
        if (!this->isValidPosition(x, y)) {
            return;
        }

        bool isBomb = this->_isBombAt(x, y);

        if (isBomb) {
            this->rows[x][y] = BOMB_STR;
        } else {
            this->_handleSafeTouch(x, y);
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
            std::cout << "🛑 Ops... Triggered a bomb! 💢💢💢" << std::endl;
            break;
        }

        std::cout << "Select X and Y (separated by spaces): ";
        std::cin.clear();
        std::cin >> x >> y;
        field.touchAt(x, y);
    }

    return 0;
}
