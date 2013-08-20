#include "usersudoku.h"
#include <locale>

int main() {
    setlocale(LC_ALL, "");
    UserSudoku game;
    game.display_greetings();

    while (true) {
        game.menu();
        game.generate();
        game.play();
        game.win();
    }

    return 0;
}
