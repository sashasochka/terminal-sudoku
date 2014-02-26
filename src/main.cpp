#include "usersudoku.h"

int main() {
    std::setlocale(LC_ALL, "");
    UserSudoku game;
    game.display_greetings();

    while (true) {
        game.menu();
        game.generate();
        game.play();
        game.win_congrats();
    }
}
