#include "sudoku.h"

#include <algorithm>
#include <ctime>

bool Sudoku::set_size(int size) {
    if (size < 2 || size > 4) {
        return false;
    }

    m_side_size = size;
    field_side_size = size * size;
    field.assign(field_side_size, std::vector<short> (field_side_size));
    return true;
}

bool Sudoku::set_level(unsigned int new_level) {
    if (new_level > 3) {
        return false;
    }

    level = new_level;
    return true;
}

bool Sudoku::set_view_type(int type) {
    if (!type || type > 3) {
        return false;
    }
    view_type = true;
    return view_type;
}

void Sudoku::fill_random() {
    srand(static_cast<unsigned int>(time(0)));

    while (true) {
        try {
            field.assign(field_side_size, std::vector<short>(field_side_size));

            for (auto k = 0; k < field_side_size * field_side_size; k++) {
                Coord coord = min_variants();

                if (!put_random(coord.x, coord.y)) {
                    throw 1;
                }
            }

            break;
        } catch(int err) {
            if (err == 1) {
                continue;
            }
        }
    }
}

Sudoku::Coord Sudoku::min_variants() {
    Coord min_el{0, 0};
    auto min_vars = 2000000000;

    for (auto y = 0; y < field_side_size; y++) {
        for (auto x = 0; x < field_side_size; x++) {
            auto vars = 0;

            for (auto i = 0; i < field_side_size; i++)
                if (vars < min_vars && !in_row(y, i + 1) && !in_column(x, i + 1) && !in_square(x, y, i + 1) && !field[y][x]) {
                    vars++;
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

void Sudoku::random_copy2userfield() {
    auto max_cells = static_cast<double>(field_side_size * field_side_size);
    std::vector<double> levels{max_cells, max_cells / 1.2, max_cells / 1.4, max_cells / 2};
    userfield.assign(field_side_size, std::vector<Cell>(field_side_size));

    for (auto i = 0; i < levels[level]; i++) {
        auto x = rand() % field_side_size;
        auto y = rand() % field_side_size;

        if (userfield[y][x].status == Status::closed) {
            userfield[y][x].status = Status::opened;
            userfield[y][x].value = field[y][x];
        } else {
            --i;
        }
    }
}

bool Sudoku::put_random(int x, int y) {
    std::vector<bool> exception(field_side_size);

    while (auto n = rand() % field_side_size + 1) {
        if (
            in_row    (y, n) ||
            in_column (x, n) ||
            in_square (x, y, n)
       ) {
            exception[n - 1] = true;

            if (std::find(exception.begin(), exception.end(), false) == exception.end()) {
                return false;
            }
        } else {
            field[y][x] = n;
            break;
        }
    }

    return true;
}

void Sudoku::user_out(std::ostream& out_stream) {
    auto nl = std::string(
        view_type == 2 || view_type == 3 ?
            field_side_size > 9 ?
                3 :
                2 :
                0, ' ') +
            std::string(field_side_size * (field_side_size > 9 ? 3 : 1) + m_side_size - 1, '-');

    if (view_type == 2 || view_type == 3) {
        out_stream << std::string(2 + (field_side_size > 9), ' ');

        for (auto i = 0; i < field_side_size; i++) {
            out_stream.width(field_side_size > 9 ? 3 : 1);

            if (view_type == 2) {
                out_stream << i + 1;
            } else {
                out_stream << static_cast<char>(i + 'a');
            }

            if (!((i + 1) % m_side_size) && i != field_side_size - 1) {
                out_stream << " ";
            }
        }

        std::cout << std::endl;
    }

    for (auto i = 0; i < field_side_size; i++) {
        if (view_type == 2 || view_type == 3) {
            out_stream.width(field_side_size > 9 ? 2 : 1);
            std::cout << i + 1 << ".";
        }

        for (auto j = 0; j < field_side_size; j++) {
            out_stream.width(field_side_size > 9 ? 3 : 1);
            out_stream << userfield[i][j].value;

            if (!((j + 1) % m_side_size) && j != field_side_size - 1) {
                std::cout << "|";
            }
        }

        out_stream << std::endl;

        if (!((i + 1) % m_side_size) && i != field_side_size - 1) {
            std::cout << nl << std::endl;
        }
    }
}

bool Sudoku::in_row(int row_index, int value) {
    for (auto i = 0; i < field_side_size; i++)
        if (field[row_index][i] == value) {
            return true;
        }

    return false;
}

bool Sudoku::in_column(int column_index, int value) {
    for (auto i = 0; i < field_side_size; i++) {
        if (field[i][column_index] == value) {
            return true;
        }
    }
    return false;
}

bool Sudoku::in_square(int x, int y, int value) {
    x = x / m_side_size * m_side_size;
    y = y / m_side_size * m_side_size;

    for (auto i = 0; i < m_side_size; i++) {
        for (auto j = 0; j < m_side_size; j++) {
            if (field[y + i][x + j] == value) {
                return true;
            }
        }
    }

    return false;
}

void Sudoku::pause() {
    std::cin.get();
}

void Sudoku::generate() {
    fill_random();
    random_copy2userfield();
    last_changed.y = -1;
    last_changed.x = -1;
}

void Sudoku::cls() {
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

bool Sudoku::userchange(unsigned int x, unsigned int y, unsigned int n) {
    if (
        n > static_cast<unsigned int>(field_side_size) ||
        x >= static_cast<unsigned int>(field_side_size) ||
        y >= static_cast<unsigned int>(field_side_size) ||
        userfield[y][x].status == Status::opened
   ) {
        return false;
    }

    last_changed.x = x;
    last_changed.y = y;
    userfield[y][x].value = n;
    return true;
}

bool Sudoku::is_won() {
    for (auto y = 0; y < field_side_size; y++) {
        for (auto x = 0; x < field_side_size; x++) {
            if (!userfield[y][x].value) {
                return false;
            }

            for (auto i = 0; i < field_side_size; i++)
                if (userfield[y][i].value == userfield[y][x].value && x != i) {
                    return false;
                }

            for (auto i = 0; i < field_side_size; i++)
                if (userfield[i][x].value == userfield[y][x].value && y != i) {
                    return false;
                }

            auto n_x = x / m_side_size * m_side_size;
            auto n_y = y / m_side_size * m_side_size;

            for (auto i = 0; i < m_side_size; i++)
                for (auto j = 0; j < m_side_size; j++)
                    if (userfield[n_y + i][n_x + j].value == userfield[y][x].value &&
                            (x != n_x + j || y != n_y + i)) {
                        return false;
                    }
        }
    }

    return true;
}

int Sudoku::getlevel() {
    return level;
}

int Sudoku::get_m_side_size() {
    return m_side_size;
}

int Sudoku::get_field_type() {
    return view_type;
}
