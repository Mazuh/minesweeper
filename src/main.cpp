#ifndef MAIN_CPP
#define MAIN_CPP

#include <string>
#include <iostream>
#include <algorithm>
#include "constants.cpp"
#include "minefield.cpp"

bool is_number(const std::string& s)
{
    return std::find_if(s.begin(), s.end(),
        [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

int main() {
    srand(time(NULL));
    std::string input_x, input_y;
    int x, y;
    char operation;
    std::string message = "Let's wait for your first move. Hope you're not nervous.";

    Game::MineField field (16, 9, 15);

    while (true) {
        std::system("clear || cls");

        if (field.isBombTouched()) {
            field.revealAll();
        }

        std::cout
            << std::endl
            << "🕵 Minesweeper | Source at: https://github.com/Mazuh/minesweeper"
            << std::endl
            << std::endl
            << field.toString()
            << std::endl;

        if (field.isBombTouched()) {
            std::cout << Game::BOMB_STR << " Ops... Triggered a bomb! Game over. 🛑" << std::endl;
            break;
        } else if (field.isAllBombsFlagged()) {
            std::cout << "🥳 Yeeey! All mines properly marked. Nice!" << std::endl;
            break;
        }

        std::cout
            << message << std::endl
            << std::endl
            << "Bomb flags: "
            << field.getUsedBombFlagsQtt() << "/" << field.getBombsQtt() << std::endl
            << std::endl
            << "Separated by spaces, insert two integer coordinates" << std::endl
            << "for X and Y axis and an operation type (x/?/!/c): ";
        std::cin.clear();
        std::cin >> input_x >> input_y;
        std::cin >> operation;

        if (!is_number(input_x) || !is_number(input_y)) {
            message = "🚫 Coordinate inputs should be integer numbers.";
            continue;
        }

        x = std::stoi(input_x);
        y = std::stoi(input_y);

        if (operation == 'x' || operation == 'X') {
            if (field.touch(x, y)) {
                message = "...";
            } else {
                message = "🚫 Failed to touch that position.";
            }
        } else if (operation == 'c' || operation == 'C') {
            if (field.setEmptyFlag(x, y)) {
                message = "⚪ Cleared flag in the given position.";
            } else {
                message = "🚫 Failed to clear flag in the given position.";
            }
        } else if (operation == '?') {
            if (field.addQuestionFlag(x, y)) {
                message = Game::QUESTION_BOMB_FLAG_STR + " Placed a question mark.";
            } else {
                message = "🚫 Failed to place question mark.";
            }
        } else if (operation == '!') {
            if (field.addBombFlag(x, y)) {
                message = Game::BOMB_FLAG_STR + " One bomb flag used.";
            } else {
                message = "🚫 Failed to use bomb flag.";
            }
        } else if (operation == 'q') {
            field.revealAll();
            message = "🃏 Secret quit (q) option called. Field revealed. Use Ctrl+C to cancel.";
        } else {
            message = "🚫 Unknowm operation.";
        }
    }

    return 0;
}

#endif
