#include "board.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>

Board Board::create_goal(const unsigned size)
{
    std::vector<std::vector<unsigned>> goal(size, std::vector<unsigned>(size));
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            goal[i][j] = j + 1 + i * size;
        }
    }
    if (size != 0) {
        goal[size - 1][size - 1] = 0;
    }
    return Board(goal);
}

Board::Board(const unsigned size)
{
    table.resize(size, std::vector<unsigned>(size, 0));
    std::vector <unsigned> cells;
    for (size_t i = 0; i < size * size; cells.push_back(i++));

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(std::begin(cells), std::end(cells), g);

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            table[i][j] = cells[i * size + j];
        }
    }
}

Board::Board(const std::vector<std::vector<unsigned>> & data)
{
    table = data;
}

std::size_t Board::size() const
{
    return table.size();
}

bool Board::is_goal() const
{
    for (size_t i = 0; i < size(); ++i) {
        for (size_t j = 0; j < size(); ++j) {
            unsigned cell = table[i][j];
            if ((cell != j + 1 + i * size() && cell != 0) || (cell == 0 && (i != size() - 1 || j != size() - 1))) {
                return false;
            }
        }
    }
    return true;
}

unsigned Board::hamming() const
{
    unsigned ans = 0;
    for (size_t i = 0; i < size(); ++i) {
        for (size_t j = 0; j < size(); ++j) {
            unsigned cell = table[i][j];
            if ((cell != j + 1 + i * size() && cell != 0) || (cell == 0 && (i != size() - 1 || j != size() - 1))) {
                    ++ans;
            }
        }
    }
    return ans;
}

unsigned Board::manhattan() const
{
    unsigned ans = 0;
    for (size_t i = 0; i < size(); ++i) {
        for (size_t j = 0; j < size(); ++j) {
            unsigned cell = table[i][j];
            if (cell != 0) {
                --cell;
                ans +=  std::max(cell / size(), i) - std::min(cell / size(), i) + std::max(cell % size(), j) - std::min(cell % size(), j);
            }
        }
    }
    return ans;
}

std::string Board::to_string() const
{
    std::string ans;
    for (size_t i = 0; i < size(); ++i) {
        for (size_t j = 0; j < size(); ++j) {
            ans += std::to_string(table[i][j]) + " ";
        }
        ans += "\n";
    }

    return ans;
}

bool Board::is_solvable() const
{
    unsigned ans = 0;
    for (size_t i = 0; i < size() * size(); ++i) {
        unsigned cur = table[i / size()][i % size()];
        if (cur == 0 && size() % 2 == 0) {
            ans += i / size() + 1;
        } else {
            for (size_t j = i + 1;  j < size() * size(); ++j) {
                unsigned comp = table[j / size()][j % size()];
                if (comp != 0 && comp < cur) {
                    ++ans;
                }
            }
        }
    }
    return ans % 2 == 0;
}

const std::vector<unsigned> & Board::operator [] (const std::size_t i) const
{
    return table[i];
}

void Board::swap_cells (unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
    std::swap(table[x1][y1], table[x2][y2]);
}
