#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>

const std::string BOMB_STR = "💣";

const std::string UNKNOWN_STR = "❓";

// const std::string BLANK_STR = "❓";

const std::string FIELD_HORIZONTAL_WALL_STR = "🧱";

const std::string FIELD_VERTICAL_WALL_STR = "🧱";

class Field {
private:
    int width;
    int height;
    int bombsQtt;
    std::vector<std::vector<std::string>> rows;
    void fillEmpty() {
        for (int iRows = 0; iRows < width; ++iRows) {
            std::vector<std::string> cols;
            for (int iCols = 0; iCols < height; ++iCols) {
                cols.push_back(UNKNOWN_STR);
            }
            this->rows.push_back(cols);
        }
    }
    void fillBombs() {
        int placedBombsQtt = 0;
        while (placedBombsQtt < this->bombsQtt) {
            int randomHorizontalPosition = rand() % this->width;
            int randomVerticalPosition = rand() % this->height;
            if (this->rows[randomHorizontalPosition][randomVerticalPosition] != BOMB_STR) {
                this->rows[randomHorizontalPosition][randomVerticalPosition] = BOMB_STR;
                placedBombsQtt++;
            }
        }
    }
public:
    Field(int width, int height, int bombsQtt) {
        this->width = width;
        this->height = height;
        this->bombsQtt = bombsQtt;

        this->fillEmpty();
        // this->fillBombs();
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
    std::cout << std::endl << "🕵 Minesweeper, the game!" << std::endl << std::endl;

    Field field (8, 8, 10);

    std::cout << field.toString() << std::endl;

    return 0;
}
