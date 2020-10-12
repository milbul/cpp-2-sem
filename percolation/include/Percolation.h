#pragma once

#include <stdio.h>
#include <vector>

struct Percolation
{
private:

    enum Type {
        CLOSED, OPEN, FULL
    };

    std::vector<std::vector<Type> > table;

    size_t open_cells;

    bool check_cell(size_t row, size_t column) const;

    void dfs(size_t row, size_t column);

    const std::vector<std::pair<int, int> > direction = {
            {0, 0},
            {-1, 0},
            {0, -1},
            {1, 0},
            {0, 1},
    };

public:

    /**
     * Construct a new Percolation object
     * @param dimension dimension of percolation table
     */
    Percolation(size_t dimension);
    /**
     * Opens the cell[row, column] if it's not opened already
     * @param row row index
     * @param column column index
     */
    void open(size_t row, size_t column);

    /**
     * Checks if cell[row, column] is open
     * @param row row index
     * @param column column index
     * @return true cell is open
     * @return false cell is blocked
     */
    bool is_open(size_t row, size_t column) const;

    /**
     * Checks if cell[row, column] is full
     * @param row row index
     * @param column column index
     * @return true cell is full
     * @return false cell is not full
     */
    bool is_full(size_t row, size_t column) const;

    /**
     * Gets number of open cells in table
     * @return number of open cells
     */
    size_t get_numbet_of_open_cells() const;

    /**
     * Checks if system percolates
     * @return true system percolates
     * @return false system doesn't percolate
     */
    bool has_percolation() const;
};