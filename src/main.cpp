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
    int width;
    int height;
    int bombsQtt;
    std::vector<std::vector<std::string>> rows;
    std::vector<std::tuple<int, int>> bombs;
    void fillEmpty() {
        for (int iRows = 0; iRows < width; ++iRows) {
            std::vector<std::string> cols;
            for (int iCols = 0; iCols < height; ++iCols) {
                cols.push_back(UNKNOWN_STR);
            }
            this->rows.push_back(cols);
        }
    }
    void generateBombs() {
        while ((int) this->bombs.size() < this->bombsQtt) {
            int randomX = rand() % this->width;
            int randomY = rand() % this->height;
            if (!this->isBombAt(randomX, randomY)) {
                this->bombs.push_back(std::make_tuple(randomX, randomY));
            }
        }
    }
    bool isBombAt(int x, int y) {
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
        this->width = width;
        this->height = height;
        this->bombsQtt = bombsQtt;

        this->fillEmpty();
        this->generateBombs();
    }
    bool touchAt(int x, int y) {
        bool isBomb = this->isBombAt(x, y);

        if (isBomb) {
            this->rows[x][y] = BOMB_STR;
        } else {
            this->rows[x][y] = BLANK_STR;
        }

        return !isBomb;
    }
    std::string toString() {
        std::stringstream stringified;

        for (int i = 0; i < this->width + 2; i++) {
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

        for (int i = 0; i < this->width + 2; i++) {
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
        std::cout << std::endl << "🕵 Minesweeper, the game!" << std::endl << std::endl;
        std::cout << field.toString() << std::endl;
        std::cout << "Select X and Y (separated by spaces): ";
        std::cin >> x;
        std::cin >> y;
        field.touchAt(x, y);
    }

    return 0;
}
