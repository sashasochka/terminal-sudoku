#include "usersudoku.h"

int main() {
    std::setlocale(LC_ALL, "");
    UserSudoku game;
    game.displayGreetings();

    while (true) {
        game.menu();
        game.generate();
        game.play();
        game.displayCongratulations();
    }
}
