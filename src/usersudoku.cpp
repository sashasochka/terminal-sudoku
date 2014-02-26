#include "usersudoku.h"
#include <cstdlib>
#include <fstream>
#include <sstream>

void UserSudoku::display_greetings() {
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

void UserSudoku::menu() {
    while (true) {
        std::cout << "Menu:\n";

        if (now_playing) {
            std::cout << "1. Return\n";
        } else {
            std::cout << "1. Play\n";
        }

        std::cout << "2. Settings\n";
        std::cout << "3. Exit\n";

        if (now_playing) {
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
                win_congrats();
                break;
            case 1:
                break;
            case 2:
                settings_menu();
                continue;
            case 3:
                exit(0);
        }

        break;
    }
}

void UserSudoku::set_size() {
    int field_size;
    std::cout << "Choose field size (2-4):\n";

    for (std::cin >> field_size; !Sudoku::set_size(field_size); std::cin >> field_size) {
        std::cout << "Incorrect size. Type again: ";
        std::cin.clear();
        std::cin.sync();
    }

    save_setting("Field size", field_size);
}

void UserSudoku::set_level() {
    unsigned short level;
    std::cout << "Set your level(1-3): ";

    for (std::cin >> level; !Sudoku::set_level(level); std::cin >> level) {
        std::cout << "Incorrect level. Type again: ";
        std::cin.clear();
        std::cin.sync();
    }

    std::cout << "\n";
    save_setting("Level", level);
}

void UserSudoku::set_view_type() {
    unsigned short type;
    std::cout << "\n";
    std::cout << "Field view types:\n";
    std::cout << "1. No labels\n";
    std::cout << "2. Labels with numbers on left and top side\n";
    std::cout << "3. Labels with numbers on left side and letters on top\n\n";
    std::cout << "Select type> ";

    for (std::cin >> type; !Sudoku::set_view_type(type); std::cin >> type) {
        std::cout << "Incorrect type. Select again> ";
        std::cin.clear();
        std::cin.sync();
    }

    std::cout << "\n";
    save_setting("View type", type);
}

void UserSudoku::save_setting(std::string setting_name, int value) {
    std::stringstream s;
    s << value;
    s >> settings[setting_name];
    // settings[setting_name] = itoa(value, const_cast<char *>(settings[setting_name].c_str()), 10);
    std::ofstream out(settings_file_path);

    for (std::map<std::string, std::string>::iterator it = settings.begin(); it != settings.end(); it++) {
        out << it->first << ": ";

        if (it->first == setting_name) {
            out << value;
        } else {
            out << it->second;
        }

        out << "\n";
    }

    out.close();
}

void UserSudoku::user_turn() {
    static bool OK = true;
    std::cin.clear();
    std::cin.sync();
    unsigned int x, y, n;
    char x_c;
    cls();
    user_out();
    std::cout << "\n";

    if (!OK) {
        std::cout << "Cell has predefined value or you entered incorrect data.\n";
    }

    std::cout << "Enter coordinates and value or menu> ";

    switch (get_field_type()) {
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
            exit(1);
    }

    OK = !std::cin.fail() && userchange(x - 1, y - 1, n);
}

void UserSudoku::win_congrats() {
    cls();
    user_out();
    std::cout << "\n";
    wins++;
    std::cout << "Greetings! You have won";

    if (wins > 1) {
        std::cout << " " << wins << " times";
    }

    std::cout << "!\n\n";
}

void UserSudoku::settings_menu() {
    while (true) {
        cls();
        std::cout << "Options:\n";
        std::cout << " 1. Level(" << getlevel() << ")\n";
        std::cout << " 2. Field size(" << get_m_side_size() << ")\n";
        std::cout << " 3. Field type(" << get_field_type() << ")\n";
        std::cout << " 4. Back\n";
        enum {
            level = 1, field, view_type, back
        };
        int settings_item;
        bool first_time = true;

        do {
            std::cout << (first_time ? "Select> " : "Try again> ");
            std::cin.sync();
            std::cin.clear();
            std::cin >> settings_item;
            first_time = false;
        } while (std::cin.bad() || settings_item < 1 || settings_item > back);

        switch (settings_item) {
            case level:
                set_level();
                break;
            case field:
                set_size();
                break;
            case view_type:
                set_view_type();
                break;
            case back:
                cls();
                return;
        }
    }
}

void UserSudoku::play() {
    now_playing = true;

    while (!is_won()) {
        user_turn();
    }

    now_playing = false;
}

void UserSudoku::init_settings() {
    settings_file_path = "~/.sudokusochkarc";
    std::ifstream in(settings_file_path);

    if (!in) {
        return;
    }

    std::string setting_name;

    while (std::getline(in, setting_name, ':')) {
        while (in.peek() == ' ') {
            in.get();
        }

        std::getline(in, settings[setting_name], '\n');
    }

    in.close();
    Sudoku::set_level(atoi(settings["Level"].c_str()));
    Sudoku::set_size(atoi(settings["Field size"].c_str()));
    Sudoku::set_view_type(atoi(settings["View type"].c_str()));
}

