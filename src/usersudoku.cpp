#include "usersudoku.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

void UserSudoku::display_greetings()
{
  cout << "    ^^^^^  ^       ^  ^^^^^^        ^^^^^    ^     ^  ^       ^   Created by " << endl;
  cout << "  ^        ^       ^  ^     ^      ^     ^   ^    ^   ^       ^   Sochka     " << endl;
  cout << " ^         ^       ^  ^      ^     ^     ^   ^   ^    ^       ^   Olexandr   " << endl;
  cout << "  ^        ^       ^  ^       ^    ^     ^   ^  ^     ^       ^              " << endl;
  cout << "    ^      ^       ^  ^       ^    ^     ^   ^^^      ^       ^              " << endl;
  cout << "      ^    ^       ^  ^       ^    ^     ^   ^  ^     ^       ^              " << endl;
  cout << "        ^  ^       ^  ^      ^     ^     ^   ^   ^    ^       ^              " << endl;
  cout << "       ^   ^       ^  ^     ^      ^     ^   ^    ^   ^       ^              " << endl;
  cout << " ^^^^^^     ^^^^^^^   ^^^^^^        ^^^^^    ^     ^   ^^^^^^^               " << endl;
  cout << endl;
}

void UserSudoku::menu()
{
  while(true) {
    cout << "Menu:        " << endl;

    if(now_playing) {
      cout << "1. Return" << endl;
    } else {
      cout << "1. Play" << endl;
    }

    cout << "2. Settings  " << endl;
    cout << "3. Help      " << endl;
    cout << "4. About     " << endl;
    cout << "5. Exit      " << endl;

    if(now_playing) {
      cout << "0. Replay" << endl;
    }

    cout << endl;
    int menu_item;
    bool first_time = true;

    do {
      cout << (first_time ? "Select> " : "Try again> ");
      cin.sync();
      cin.clear();
      cin >> menu_item;
      first_time = false;
    } while(cin.bad() || menu_item < 0 || menu_item > 5);

    cout << endl;

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

bool UserSudoku::replay(istream& in, ostream& out)
{
  static bool first_time = true;

  if(first_time) {
    first_time = false;
    return true;
  }

  out << "Do you want to play again (y/n)?" << endl;
  char again;
  in >> again;
  return again == 'y';
}

void UserSudoku::set_size()
{
  int field_size;
  cout << "Choose field size (2-4): " << endl;

  for(cin >> field_size; !Sudoku::set_size(field_size); cin >> field_size) {
    cout << "Incorrect size. Type again: ";
    cin.clear();
    cin.sync();
  }

  save_setting("Field size", field_size);
}

void UserSudoku::set_level()
{
  unsigned short level;
  cout << "Set your level(1-3): ";

  for(cin >> level; !Sudoku::set_level(level); cin >> level) {
    cout << "Incorrect level. Type again: ";
    cin.clear();
    cin.sync();
  }

  cout << endl;
  save_setting("Level", level);
}

void UserSudoku::set_view_type()
{
  unsigned short type;
  cout << endl;
  cout << "Field view types: " << endl;
  cout << "1. No labels" << endl;
  cout << "2. Labels with numbers on left and top side" << endl;
  cout << "3. Labels with numbers on left side and letters on top" << endl << endl;
  cout << "Select type> ";

  for(cin >> type; !Sudoku::set_view_type(type); cin >> type) {
    cout << "Incorrect type. Select again> ";
    cin.clear();
    cin.sync();
  }

  cout << endl;
  save_setting("View type", type);
}

void UserSudoku::save_setting(string setting_name, int value)
{
  stringstream s;
  s << value;
  s >> settings[setting_name];
  // settings[setting_name] = itoa(value, const_cast<char *>(settings[setting_name].c_str()), 10);
  ofstream out(settings_file_path);

  for(map<string, string>::iterator it = settings.begin(); it != settings.end(); it++) {
    out << it->first << ": ";

    if(it->first == setting_name) {
      out << value;
    } else {
      out << it->second;
    }

    out << endl;
  }

  out.close();
}

void UserSudoku::user_turn()
{
  static bool OK = true;
  cin.clear();
  cin.sync();
  unsigned int x, y, n;
  char x_c;
  cls();
  user_out();
  cout << endl;

  if(!OK) {
    cout << "Cell has predefined value or you entered incorrect data." << endl;
  }

  cout << "Enter coordinates and value or menu> ";

  switch(get_field_type()) {
  case 1:
  case 2:
    cin >> y >> x >> n;

    if(cin.fail()) {
      cin.clear();
      string str;
      cin >> str;

      if(str == "menu") {
        menu();
        OK = true;
        return;
      } else if(str == "help") {
        help();
        OK = true;
        return;
      } else if(str == "about") {
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

    if(cin.fail()) {
      cin.clear();
      string str;
      cin >> str;
      str = x_c + str;

      if(str == "menu") {
        menu();
        OK = true;
        return;
      } else if(str == "help") {
        help();
        OK = true;
        return;
      } else if(str == "about") {
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
    cout << "Error: this view type is not supported." << endl;
    exit(1);
  }

  OK = !cin.fail() && userchange(x - 1, y - 1, n) ;
}

void UserSudoku::win()
{
  cls();
  user_out();
  cout << endl;
  wins++;
  cout << "Greetings! You have won";

  if(wins > 1) {
    cout << " " << wins << " times";
  }

  cout << "!" << endl << endl;
}

void UserSudoku::settings_menu()
{
  while(true) {
    cls();
    cout << "Options: " << endl;
    cout << " 1. Level(" << getlevel() << ")" << endl;
    cout << " 2. Field size(" << get_m_side_size() << ")" << endl;
    cout << " 3. Field type(" << get_field_type() << ")" << endl;
    cout << " 4. Back" << endl;
    enum {level = 1, field, view_type, back};
    int settings_item;
    bool first_time = true;

    do {
      cout << (first_time ? "Select> " : "Try again> ");
      cin.sync();
      cin.clear();
      cin >> settings_item;
      first_time = false;
    } while(cin.bad() || settings_item < 1 || settings_item > back);

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

void UserSudoku::help()
{
  cls();
  cout << "SUDOKU HELP" << endl << endl;
  cout << "Description: Japanese logical game;" << endl << endl;
  cout << "Target:      correctly set all cells in grid;" << endl << endl;
  cout << "Rules: " << endl << endl;
  cout << "  -at start there are predefined cells(green color). You can't change them;    " << endl;
  cout << "  -other cells are free (grey color) and their values are 0;                   " << endl;
  cout << "  -every turn you must change free and already cheanged cells;                 " << endl;
  cout << "  -to change it you have to enter 2 coords and value of it;                    " << endl;
  cout << "  -last changed cell has purple color, other have yellow color;                " << endl;
  cout << "  -in every column, row and smaller grid must be only one each number;         " << endl;
  cout << "  -when user entered all cells and they are correct- he wins;                  " << endl << endl;
  cout << "Press enter to return" << endl;
  pause();
  cls();
}

void UserSudoku::about()
{
  cls();
  cout << "This game was created by Sochka Olexandr." << endl;
  cout << "Support: sasha.sochka@gmail.com" << endl;
  pause();
  cls();
}

void UserSudoku::play()
{
  now_playing = true;

  while(!is_won()) {
    user_turn();
  }

  now_playing = false;
}

void UserSudoku::init_settings()
{
  settings_file_path = "~/.sudokusochkarc";
  ifstream in(settings_file_path);

  if(!in) {
    return;
  }

  string setting_name;

  while(getline(in, setting_name, ':')) {
    while(in.peek() == ' ') {
      in.get();
    }

    getline(in, settings[setting_name], '\n');
  }

  in.close();
  Sudoku::set_level       (atoi(settings["Level"]     .c_str()));
  Sudoku::set_size        (atoi(settings["Field size"].c_str()));
  Sudoku::set_view_type   (atoi(settings["View type"] .c_str()));
}
