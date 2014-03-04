#include "usersudoku.h"
#include <cassert>
#include <fstream>
#include <sstream>

UserSudoku::UserSudoku(): nWins{0}, nowPlaying{false} {
    initSettings();
}

void UserSudoku::clearDisplay() {
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

void UserSudoku::displayCongratulations() {
    clearDisplay();
    sudoku.displayField();
    std::cout << "\n";
    ++nWins;
    std::cout << "Greetings! You have won";

    if (nWins > 1) {
        std::cout << " " << nWins << " times";
    }

    std::cout << "!\n\n";
}

void UserSudoku::displayGreetings() {
    std::cout << R"(
        ^^^^^  ^       ^  ^^^^^^        ^^^^^    ^     ^  ^       ^   Created by
      ^        ^       ^  ^     ^      ^     ^   ^    ^   ^       ^   Sochka
     ^         ^       ^  ^      ^     ^     ^   ^   ^    ^       ^   Olexandr
      ^        ^       ^  ^       ^    ^     ^   ^  ^     ^       ^
        ^      ^       ^  ^       ^    ^     ^   ^^^      ^       ^
          ^    ^       ^  ^       ^    ^     ^   ^  ^     ^       ^
            ^  ^       ^  ^      ^     ^     ^   ^   ^    ^       ^
           ^   ^       ^  ^     ^      ^     ^   ^    ^   ^       ^
     ^^^^^^     ^^^^^^^   ^^^^^^        ^^^^^    ^     ^   ^^^^^^^
     )";
}

void UserSudoku::displaySettingsMenu() {
    while (true) {
        clearDisplay();
        std::cout << "Options:\n";
        std::cout << " 1. Level(" << sudoku.getLevel() << ")\n";
        std::cout << " 2. Field size(" << sudoku.getSideSize() << ")\n";
        std::cout << " 3. Field type(" << sudoku.getFieldType() << ")\n";
        std::cout << " 4. Back\n";
        enum {
            level = 1, field, view_type, back
        };
        int settingsItem;
        bool firstTime = true;

        do {
            std::cout << (firstTime ? "Select> " : "Try again> ");
            std::cin.sync();
            std::cin.clear();
            std::cin >> settingsItem;
            firstTime = false;
        } while (std::cin.bad() || settingsItem < 1 || settingsItem > back);

        switch (settingsItem) {
            case level:
                setLevel();
                break;
            case field:
                setSize();
                break;
            case view_type:
                setViewType();
                break;
            case back:
                clearDisplay();
                return;
        }
    }
}

void UserSudoku::generate() {
    sudoku.generate();
}

void UserSudoku::initSettings() {
    settingsPath = "~/.sudokusochkarc";
    std::ifstream in(settingsPath);

    if (!in) {
        return;
    }

    std::string settingName;

    while (std::getline(in, settingName, ':')) {
        while (in.peek() == ' ') {
            in.get();
        }

        std::getline(in, settings[settingName], '\n');
    }

    in.close();
    sudoku.setLevel(stoul(settings["Level"]));
    sudoku.setSize(stoul(settings["Field size"]));
    sudoku.setViewType(stoul(settings["View type"]));
}

void UserSudoku::menu() {
    while (true) {
        std::cout << "Menu:\n";

        if (nowPlaying) {
            std::cout << "1. Return\n";
        } else {
            std::cout << "1. Play\n";
        }

        std::cout << "2. Settings\n";
        std::cout << "3. Exit\n";

        if (nowPlaying) {
            std::cout << "0. Replay\n";
        }

        std::cout << "\n";
        int menu_item;
        bool first_time = true;

        do {
            std::cout << (first_time ? "Select> " : "Try again> ");
            std::cin.sync();
            std::cin.clear();
            std::cin >> menu_item;
            first_time = false;
        } while (std::cin.bad() || menu_item < 0 || menu_item > 5);

        std::cout << "\n";

        switch (menu_item) {
            case 0:
                generate();
                play();
                displayCongratulations();
                break;
            case 1:
                break;
            case 2:
                displaySettingsMenu();
                continue;
            case 3:
                exit(0);
            default:
                assert(false);
        }

        break;
    }
}

void UserSudoku::saveSetting(const std::string& settingName, int value) {
    std::stringstream s;
    s << value;
    s >> settings[settingName];
    std::ofstream out(settingsPath);

    for (const auto& setting: settings) {
        out << setting.first << ": ";

        if (setting.first == settingName) {
            out << value;
        } else {
            out << setting.second;
        }

        out << "\n";
    }

    out.close();
}

void UserSudoku::setLevel() {
    unsigned short level;
    std::cout << "Set your level(1-3): ";

    for (std::cin >> level; !sudoku.setLevel(level); std::cin >> level) {
        std::cout << "Incorrect level. Type again: ";
        std::cin.clear();
        std::cin.sync();
    }

    std::cout << "\n";
    saveSetting("Level", level);
}

void UserSudoku::setSize() {
    int fieldSize;
    std::cout << "Choose field size (2-4):\n";

    for (std::cin >> fieldSize; !sudoku.setSize(fieldSize); std::cin >> fieldSize) {
        std::cout << "Incorrect size. Type again: ";
        std::cin.clear();
        std::cin.sync();
    }

    saveSetting("Field size", fieldSize);
}

void UserSudoku::setViewType() {
    unsigned short type;
    std::cout << "\n";
    std::cout << "Field view types:\n";
    std::cout << "1. No labels\n";
    std::cout << "2. Labels with numbers on left and top side\n";
    std::cout << "3. Labels with numbers on left side and letters on top\n\n";
    std::cout << "Select type> ";

    for (std::cin >> type; !sudoku.setViewType(type); std::cin >> type) {
        std::cout << "Incorrect type. Select again> ";
        std::cin.clear();
        std::cin.sync();
    }

    std::cout << "\n";
    saveSetting("View type", type);
}

void UserSudoku::userTurn() {
    static bool OK = true;
    std::cin.clear();
    std::cin.sync();
    clearDisplay();
    sudoku.displayField();
    std::cout << "\n";

    if (!OK) {
        std::cout << "Cell has predefined value or you entered incorrect data.\n";
    }

    std::cout << "Enter coordinates and value or menu> ";

    unsigned int x, y, n;
    char x_c;
    switch (sudoku.getFieldType()) {
        case 1:
        case 2:
            std::cin >> y >> x >> n;

            if (std::cin.fail()) {
                std::cin.clear();
                std::string str;
                std::cin >> str;

                if (str == "menu") {
                    menu();
                    OK = true;
                    return;
                } else {
                    std::cin.setstate(std::ios_base::badbit);
                }
            }

            break;
        case 3:
            std::cin >> x_c >> y >> n;

            if (std::cin.fail()) {
                std::cin.clear();
                std::string str;
                std::cin >> str;
                str = x_c + str;

                if (str == "menu") {
                    menu();
                    OK = true;
                    return;
                } else {
                    std::cin.setstate(std::ios_base::badbit);
                }
            }

            x = x_c - 'a' + 1;
            break;
        default:
            std::cout << "Error: this view type is not supported.\n";
            exit(EXIT_FAILURE);
    }

    OK = !std::cin.fail() && sudoku.userChange(x - 1, y - 1, n);
}

void UserSudoku::play() {
    nowPlaying = true;

    while (!sudoku.finished()) {
        userTurn();
    }

    nowPlaying = false;
}
