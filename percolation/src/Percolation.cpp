#include "Percolation.h"
#include <vector>
#include <algorithm>

Percolation::Percolation(size_t dimension) : open_cells(0)
{
    table.resize(dimension, std::vector<Type>(dimension, CLOSED));
}

bool Percolation::check_cell(size_t row, size_t column) const {
    return row < table.size() && column < table.size();
}
void Percolation::dfs(size_t row, size_t column)
{
    table[row][column] = FULL;

    for (size_t i = 0; i < direction.size(); ++i) {
        size_t cur_row = row + direction[i].first;
        size_t cur_column = column + direction[i].second;
        if (check_cell(cur_row, cur_column) && table[cur_row][cur_column] != FULL && table[cur_row][cur_column] == OPEN) {
            dfs(cur_row, cur_column);
        }
    }
}

void Percolation::open(size_t row, size_t column)
{
    if (table[row][column] != CLOSED) {
        return;
    }
    table[row][column] = OPEN;
    if (row == 0) {
        table[row][column] = FULL;
    }
    ++open_cells;
    for (size_t i = 0; i < direction.size(); ++i) {
        if (is_full(row + direction[i].first, column + direction[i].second)) {
            dfs(row + direction[i].first, column + direction[i].second);
            break;
        }
    }
}

bool Percolation::is_open(size_t row, size_t column) const
{
    return check_cell(row, column) && table[row][column];
}

bool Percolation::is_full(size_t row, size_t column) const
{
    return check_cell(row, column) && table[row][column] == FULL;
}

size_t Percolation::get_numbet_of_open_cells() const
{
    return open_cells;
}

bool Percolation::has_percolation() const
{
    return std::any_of(table[table.size() - 1].begin(), table[table.size() - 1].end(), [](int i) { return i == FULL; });
}


