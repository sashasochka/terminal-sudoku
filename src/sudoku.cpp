#include "sudoku.h"

#include <algorithm>
#include <ctime>

Sudoku::Sudoku(): sideSize{3},
                  fieldSideSize{sideSize * sideSize},
                  level{1},
                  viewType{3},
                  lastChanged{-1, -1} {
}

bool Sudoku::setSize(int size) {
    if (size < 2 || size > 4) {
        return false;
    }

    sideSize = size;
    fieldSideSize = size * size;
    field.assign(fieldSideSize, std::vector<short>(fieldSideSize));
    return true;
}

bool Sudoku::setLevel(unsigned int new_level) {
    if (new_level > 3) {
        return false;
    }

    level = new_level;
    return true;
}

bool Sudoku::setViewType(int type) {
    if (!type || type > 3) {
        return false;
    }
    viewType = 1;
    return true;
}

void Sudoku::randomFill() {
    srand(static_cast<unsigned int>(time(0)));

    while (true) {
        bool need_break = true;
        field.assign(fieldSideSize, std::vector<short>(fieldSideSize));

        for (auto k = 0; k < fieldSideSize * fieldSideSize; ++k) {
            Coord coord = minVariants();

            if (!putRandomValue(coord.x, coord.y)) {
                need_break = false;
                break;
            }
        }
        if (need_break) {
            break;
        }
    }
}

Sudoku::Coord Sudoku::minVariants() {
    Coord min_el{0, 0};
    auto min_vars = 2000000000;

    for (auto y = 0; y < fieldSideSize; ++y) {
        for (auto x = 0; x < fieldSideSize; ++x) {
            auto vars = 0;

            for (auto i = 0; i < fieldSideSize; ++i)
                if (vars < min_vars && !rowContainsValue(y, i + 1) && !columnsContainsValue(x, i + 1) && !squareContainsValue(x, y, i + 1) && !field[y][x]) {
                    ++vars;
                }

            if (vars < min_vars && vars) {
                min_vars = vars;
                min_el.x = x;
                min_el.y = y;
            }
        }
    }

    return min_el;
}

void Sudoku::randomCopyToUserField() {
    auto max_cells = static_cast<double>(fieldSideSize * fieldSideSize);
    std::vector<double> levels{max_cells, max_cells / 1.2, max_cells / 1.4, max_cells / 2};
    userField.assign(fieldSideSize, std::vector<Cell>(fieldSideSize));

    for (auto i = 0; i < levels[level]; ++i) {
        auto x = rand() % fieldSideSize;
        auto y = rand() % fieldSideSize;

        if (userField[y][x].status == Status::closed) {
            userField[y][x].status = Status::opened;
            userField[y][x].value = field[y][x];
        } else {
            --i;
        }
    }
}

bool Sudoku::putRandomValue(int x, int y) {
    std::vector<bool> exception(fieldSideSize);

    while (auto n = rand() % fieldSideSize + 1) {
        if (rowContainsValue(y, n) ||
              columnsContainsValue(x, n) ||
              squareContainsValue(x, y, n)) {
            exception[n - 1] = true;

            if (find(exception.begin(), exception.end(), false) == exception.end()) {
                return false;
            }
        } else {
            field[y][x] = n;
            break;
        }
    }

    return true;
}

void Sudoku::displayField(std::ostream &out_stream) {
    auto nl = std::string(
            viewType == 2 || viewType == 3 ?
                    fieldSideSize > 9 ?
                            3 :
                            2 :
                    0, ' ') +
            std::string(
                    fieldSideSize * (fieldSideSize > 9 ? 3 : 1) + sideSize - 1,
                    '-');

    if (viewType == 2 || viewType == 3) {
        out_stream << std::string(2 + (fieldSideSize > 9), ' ');

        for (auto i = 0; i < fieldSideSize; ++i) {
            out_stream.width(fieldSideSize > 9 ? 3 : 1);

            if (viewType == 2) {
                out_stream << i + 1;
            } else {
                out_stream << static_cast<char>(i + 'a');
            }

            if (!((i + 1) % sideSize) && i != fieldSideSize - 1) {
                out_stream << " ";
            }
        }

        std::cout << std::endl;
    }

    for (auto i = 0; i < fieldSideSize; ++i) {
        if (viewType == 2 || viewType == 3) {
            out_stream.width(fieldSideSize > 9 ? 2 : 1);
            std::cout << i + 1 << ".";
        }

        for (auto j = 0; j < fieldSideSize; ++j) {
            out_stream.width(fieldSideSize > 9 ? 3 : 1);
            out_stream << userField[i][j].value;

            if (!((j + 1) % sideSize) && j != fieldSideSize - 1) {
                std::cout << "|";
            }
        }

        out_stream << std::endl;

        if (!((i + 1) % sideSize) && i != fieldSideSize - 1) {
            std::cout << nl << std::endl;
        }
    }
}

bool Sudoku::rowContainsValue(int rowIndex, int value) {
    const auto& row = field[rowIndex];
    return find(row.begin(), row.end(), value) != row.end();
}

bool Sudoku::columnsContainsValue(int columnIndex, int value) {
    return find_if(field.begin(), field.end(), [=](std::vector<short>& row) {
        return row[columnIndex] == value;
    }) != field.end();
}

bool Sudoku::squareContainsValue(int x, int y, int value) {
    x = x / sideSize * sideSize;
    y = y / sideSize * sideSize;

    for (auto i = 0; i < sideSize; ++i) {
        for (auto j = 0; j < sideSize; ++j) {
            if (field[y + i][x + j] == value) {
                return true;
            }
        }
    }

    return false;
}

void Sudoku::generate() {
    randomFill();
    randomCopyToUserField();
    lastChanged.y = -1;
    lastChanged.x = -1;
}

bool Sudoku::userChange(unsigned int x, unsigned int y, unsigned int n) {
    if (n > static_cast<unsigned int>(fieldSideSize) ||
                    x >= static_cast<unsigned int>(fieldSideSize) ||
                    y >= static_cast<unsigned int>(fieldSideSize) ||
                    userField[y][x].status == Status::opened
            ) {
        return false;
    }

    lastChanged.x = x;
    lastChanged.y = y;
    userField[y][x].value = n;
    return true;
}

bool Sudoku::finished() {
    for (auto y = 0; y < fieldSideSize; ++y) {
        for (auto x = 0; x < fieldSideSize; ++x) {
            if (!userField[y][x].value) {
                return false;
            }

            for (auto i = 0; i < fieldSideSize; ++i)
                if (userField[y][i].value == userField[y][x].value && x != i) {
                    return false;
                }

            for (auto i = 0; i < fieldSideSize; ++i)
                if (userField[i][x].value == userField[y][x].value && y != i) {
                    return false;
                }

            auto n_x = x / sideSize * sideSize;
            auto n_y = y / sideSize * sideSize;

            for (auto i = 0; i < sideSize; ++i)
                for (auto j = 0; j < sideSize; ++j)
                    if (userField[n_y + i][n_x + j].value ==
                            userField[y][x].value &&
                            (x != n_x + j || y != n_y + i)) {
                        return false;
                    }
        }
    }

    return true;
}

int Sudoku::getLevel() {
    return level;
}

int Sudoku::getSideSize() {
    return sideSize;
}

int Sudoku::getFieldType() {
    return viewType;
}
