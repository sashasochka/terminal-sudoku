#ifndef SUDOKU_H_
#define SUDOKU_H_
#include <iostream>
#include <vector>

class Sudoku {
private:
    enum class Status {closed, opened};
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
    std::vector<std::vector<short>> field;
    std::vector<std::vector<Cell>> userfield;

    bool put_random(int x, int y);
    bool in_row(int row_index, int value);
    bool in_column(int column_index, int value);
    bool in_square(int x, int y, int value);
    void fill_random();
    void random_copy2userfield();
    Coord min_variants();
protected:
    int getlevel();
    int get_m_side_size();
    bool set_view_type(int type);
    int get_field_type();
    bool set_size(int size);
    bool set_level(unsigned int level);
public:
    Sudoku():
        m_side_size{3},
        field_side_size{m_side_size * m_side_size},
        level{1},
        view_type{3},
        last_changed{-1, -1} {}
    void generate();
    void user_out(std::ostream& out_stream = std::cout);
    void field_out(std::ostream& out_stream = std::cout);
    void pause();
    void cls();
    bool userchange(unsigned int x, unsigned int y, unsigned int n);
    bool is_won();
};

#endif // SUDOKU_H_

