#include "usersudoku.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

void UserSudoku::display_greetings() {
    cout << "    ^^^^^  ^       ^  ^^^^^^        ^^^^^    ^     ^  ^       ^   Created by\n";
    cout << "  ^        ^       ^  ^     ^      ^     ^   ^    ^   ^       ^   Sochka\n";
    cout << " ^         ^       ^  ^      ^     ^     ^   ^   ^    ^       ^   Olexandr\n";
    cout << "  ^        ^       ^  ^       ^    ^     ^   ^  ^     ^       ^   \n";
    cout << "    ^      ^       ^  ^       ^    ^     ^   ^^^      ^       ^   \n";
    cout << "      ^    ^       ^  ^       ^    ^     ^   ^  ^     ^       ^   \n";
    cout << "        ^  ^       ^  ^      ^     ^     ^   ^   ^    ^       ^   \n";
    cout << "       ^   ^       ^  ^     ^      ^     ^   ^    ^   ^       ^   \n";
    cout << " ^^^^^^     ^^^^^^^   ^^^^^^        ^^^^^    ^     ^   ^^^^^^^    \n";
    cout << "\n";
}

void UserSudoku::menu() {
    while (true) {
        cout << "Menu:\n";

        if (now_playing) {
            cout << "1. Return\n";
        } else {
            cout << "1. Play\n";
        }

        cout << "2. Settings\n";
        cout << "3. Help\n";
        cout << "4. About\n";
        cout << "5. Exit\n";

        if (now_playing) {
            cout << "0. Replay\n";
        }

        cout << "\n";
        int menu_item;
        bool first_time = true;

        do {
            cout << (first_time ? "Select> " : "Try again> ");
            cin.sync();
            cin.clear();
            cin >> menu_item;
            first_time = false;
        } while (cin.bad() || menu_item < 0 || menu_item > 5);

        cout << "\n";

        switch(menu_item) {
        case 0:
            generate();
            play();
            win();
            break;
        case 1:
            break;
        case 2:
            settings_menu();
            continue;
        case 3:
            help();
            continue;
        case 4:
            about();
            continue;
        case 5:
            exit(0);
        }

        break;
    }
}

bool UserSudoku::replay(istream& in, ostream& out) {
    static bool first_time = true;

    if (first_time) {
        first_time = false;
        return true;
    }

    out << "Do you want to play again (y/n)?\n";
    char again;
    in >> again;
    return again == 'y';
}

void UserSudoku::set_size() {
    int field_size;
    cout << "Choose field size (2-4):\n";

    for(cin >> field_size; !Sudoku::set_size(field_size); cin >> field_size) {
        cout << "Incorrect size. Type again: ";
        cin.clear();
        cin.sync();
    }

    save_setting("Field size", field_size);
}

void UserSudoku::set_level() {
    unsigned short level;
    cout << "Set your level(1-3): ";

    for(cin >> level; !Sudoku::set_level(level); cin >> level) {
        cout << "Incorrect level. Type again: ";
        cin.clear();
        cin.sync();
    }

    cout << "\n";
    save_setting("Level", level);
}

void UserSudoku::set_view_type() {
    unsigned short type;
    cout << "\n";
    cout << "Field view types:\n";
    cout << "1. No labels\n";
    cout << "2. Labels with numbers on left and top side\n";
    cout << "3. Labels with numbers on left side and letters on top\n\n";
    cout << "Select type> ";

    for(cin >> type; !Sudoku::set_view_type(type); cin >> type) {
        cout << "Incorrect type. Select again> ";
        cin.clear();
        cin.sync();
    }

    cout << "\n";
    save_setting("View type", type);
}

void UserSudoku::save_setting(string setting_name, int value) {
    stringstream s;
    s << value;
    s >> settings[setting_name];
    // settings[setting_name] = itoa(value, const_cast<char *>(settings[setting_name].c_str()), 10);
    ofstream out(settings_file_path);

    for(map<string, string>::iterator it = settings.begin(); it != settings.end(); it++) {
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
    cin.clear();
    cin.sync();
    unsigned int x, y, n;
    char x_c;
    cls();
    user_out();
    cout << "\n";

    if (!OK) {
        cout << "Cell has predefined value or you entered incorrect data.\n";
    }

    cout << "Enter coordinates and value or menu> ";

    switch(get_field_type()) {
    case 1:
    case 2:
        cin >> y >> x >> n;

        if (cin.fail()) {
            cin.clear();
            string str;
            cin >> str;

            if (str == "menu") {
                menu();
                OK = true;
                return;
            } else if (str == "help") {
                help();
                OK = true;
                return;
            } else if (str == "about") {
                about();
                OK = true;
                return;
            } else {
                cin.setstate(ios_base::badbit);
            }
        }

        break;
    case 3:
        cin >> x_c >> y >> n;

        if (cin.fail()) {
            cin.clear();
            string str;
            cin >> str;
            str = x_c + str;

            if (str == "menu") {
                menu();
                OK = true;
                return;
            } else if (str == "help") {
                help();
                OK = true;
                return;
            } else if (str == "about") {
                about();
                OK = true;
                return;
            } else {
                cin.setstate(ios_base::badbit);
            }
        }

        x = x_c - 'a' + 1;
        break;
    default:
        cout << "Error: this view type is not supported.\n";
        exit(1);
    }

    OK = !cin.fail() && userchange(x - 1, y - 1, n) ;
}

void UserSudoku::win() {
    cls();
    user_out();
    cout << "\n";
    wins++;
    cout << "Greetings! You have won";

    if (wins > 1) {
        cout << " " << wins << " times";
    }

    cout << "!\n\n";
}

void UserSudoku::settings_menu() {
    while (true) {
        cls();
        cout << "Options:\n";
        cout << " 1. Level(" << getlevel() << ")\n";
        cout << " 2. Field size(" << get_m_side_size() << ")\n";
        cout << " 3. Field type(" << get_field_type() << ")\n";
        cout << " 4. Back\n";
        enum {level = 1, field, view_type, back};
        int settings_item;
        bool first_time = true;

        do {
            cout << (first_time ? "Select> " : "Try again> ");
            cin.sync();
            cin.clear();
            cin >> settings_item;
            first_time = false;
        } while (cin.bad() || settings_item < 1 || settings_item > back);

        switch(settings_item) {
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

void UserSudoku::help() {
    cls();
    cout << "SUDOKU HELP\n\n";
    cout << "Description: Japanese logical game;\n\n";
    cout << "Target:      correctly set all cells in grid;\n\n";
    cout << "Rules:\n\n";
    cout << "  -at start there are predefined cells(green color). You can't change them;\n";
    cout << "  -other cells are free (grey color) and their values are 0;\n";
    cout << "  -every turn you must change free and already cheanged cells;\n";
    cout << "  -to change it you have to enter 2 coords and value of it;\n";
    cout << "  -last changed cell has purple color, other have yellow color;\n";
    cout << "  -in every column, row and smaller grid must be only one each number;\n";
    cout << "  -when user entered all cells and they are correct- he wins;\n\n";
    cout << "Press enter to return\n";
    pause();
    cls();
}

void UserSudoku::about() {
    cls();
    cout << "This game was created by Sochka Olexandr.\n";
    cout << "Support: sasha.sochka@gmail.com\n";
    pause();
    cls();
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
    ifstream in(settings_file_path);

    if (!in) {
        return;
    }

    string setting_name;

    while (getline(in, setting_name, ':')) {
        while (in.peek() == ' ') {
            in.get();
        }

        getline(in, settings[setting_name], '\n');
    }

    in.close();
    Sudoku::set_level(atoi(settings["Level"].c_str()));
    Sudoku::set_size(atoi(settings["Field size"].c_str()));
    Sudoku::set_view_typ(atoi(settings["View type"] .c_str()));
}
