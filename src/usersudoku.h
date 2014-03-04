#ifndef USERSUDOKU_H_
#define USERSUDOKU_H_

#include "sudoku.h"
#include <iostream>
#include <map>
#include <string>

class UserSudoku {
private:
    bool nowPlaying;
    int nWins;
    std::map<std::string, std::string> settings;
    std::string settingsPath;
    Sudoku sudoku;
public:
    UserSudoku();
    void clearDisplay();
    void displayGreetings();
    void displayCongratulations();
    void displaySettingsMenu();
    void generate();
    void initSettings();
    void menu();
    void play();
    void saveSetting(const std::string &settingName, int value);
    void setLevel();
    void setSize();
    void setViewType();
    void userTurn();
};

#endif // USERSUDOKU_H_
