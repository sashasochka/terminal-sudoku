#pragma once
#include <iostream>
#include <vector>

using std::cout;
using std::ostream;
using std::vector;

class Sudoku
{
private:
  enum Status {closed, opened, all};
  struct Cell {
    int value;
    Status status;
  };
  struct Coord {
    int x;
    int y;
  };

  int m_side_size;
  int field_side_size;
  int level;
  int view_type;
  Coord last_changed;
  vector<vector<short>> field;
  vector<vector<Cell>> userfield;

  bool put_random(int x, int y);
  bool in_row(int row_index, int value);
  bool in_column(int column_index, int value);
  bool in_square(int x, int y, int value);
  void fill_random();
  void random_copy2userfield();
  Coord min_variants();
protected:
  int  getlevel();
  int  get_m_side_size();
  bool set_view_type(int type);
  int  get_field_type();
  bool set_size(int size);
  bool set_level(unsigned int level);
public:
  Sudoku():
    m_side_size(3),
    field_side_size(m_side_size*m_side_size),
    level(1),
    view_type(3) {
    last_changed.y = -1;
    last_changed.x = -1;
  };
  void generate();
  void user_out(ostream& out_stream = cout);
  void field_out(ostream& out_stream = cout);
  int  pause();
  int  cls();
  bool userchange(unsigned int x, unsigned int y, unsigned int n);
  bool is_won();
};

