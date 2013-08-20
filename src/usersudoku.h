#ifndef USERSUDOKU_H_
#define USERSUDOKU_H_
#include "sudoku.h"
#include <iostream>
#include <map>
#include <string>

using std::cout;
using std::cin;
using std::ostream;
using std::istream;
using std::string;
using std::map;

class UserSudoku: public Sudoku {
private:
    int wins;
    string settings_file_path;
    map<string, string> settings;
    bool now_playing;
public:
    UserSudoku(): wins{0}, now_playing{false} {
        init_settings();
    }
    void display_greetings();
    void menu();
    bool replay(istream& in = cin, ostream& out = cout);
    void set_size();
    void set_level();
    void user_turn();
    void win();
    void settings_menu();
    void help();
    void about();
    void play();
    void init_settings();
    void set_view_type();
    void save_setting(string setting_name, int value);
};

#endif // USERSUDOKU_H_
