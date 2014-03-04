#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <iostream>
#include <vector>

class Sudoku {
private:
    enum class Status {
        closed, opened
    };
    struct Cell {
        int value;
        Status status;
    };
    struct Coord {
        int x;
        int y;
    };

    int sideSize;
    int fieldSideSize;
    int level;
    int viewType;
    Coord lastChanged;
    std::vector<std::vector<short>> field;
    std::vector<std::vector<Cell>> userField;

    bool putRandomValue(int x, int y);
    bool rowContainsValue(int rowIndex, int value);
    bool columnsContainsValue(int columnIndex, int value);
    bool squareContainsValue(int x, int y, int value);
    void randomFill();
    void randomCopyToUserField();
    Coord minVariants();
public:
    Sudoku();
    void displayField(std::ostream &out_stream = std::cout);
    bool finished();
    void generate();
    int getFieldType();
    int getLevel();
    int getSideSize();
    bool setLevel(unsigned int level);
    bool setSize(int size);
    bool setViewType(int type);
    bool userChange(unsigned int x, unsigned int y, unsigned int n);
};

#endif // SUDOKU_H_
