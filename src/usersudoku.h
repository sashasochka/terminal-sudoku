#ifndef USERSUDOKU_H_
#define USERSUDOKU_H_
#include "sudoku.h"
#include <iostream>
#include <map>
#include <string>

class UserSudoku: public Sudoku {
private:
    int wins;
    std::string settings_file_path;
    std::map<std::string, std::string> settings;
    bool now_playing;
public:
    UserSudoku(): wins{0}, now_playing{false} {
        init_settings();
    }
    void display_greetings();
    void menu();
    void set_size();
    void set_level();
    void user_turn();
    void win_congrats();
    void settings_menu();
    void help();
    void about();
    void play();
    void init_settings();
    void set_view_type();
    void save_setting(std::string setting_name, int value);
};

#endif // USERSUDOKU_H_
