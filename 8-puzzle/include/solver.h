#pragma once

#include "board.h"
#include <vector>

class Solver
{
public:
    explicit Solver(const Board & board);

    Solver(const Solver & other) = default;

    Solver & operator = (const Solver & other) = default;

    std::size_t moves() const;

    auto begin() const
    { return m_moves.begin(); }

    auto end() const
    { return m_moves.end(); }

private:
    std::vector<Board> m_moves;

    std::vector<std::pair<int, int> > direction = {
            {-1, 0},
            {1, 0},
            {0, -1},
            {0, 1},
    };
};
