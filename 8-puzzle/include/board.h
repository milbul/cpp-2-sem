#pragma once

#include <string>
#include <vector>

class Board
{
private:
    std::vector<std::vector<unsigned>> table;

public:
    void swap_cells(unsigned x1, unsigned y1, unsigned x2, unsigned y2);

    static Board create_goal(unsigned size);

    Board() = default;

    Board(const Board & other) = default;

    Board & operator = (const Board & other) = default;

    explicit Board(unsigned size);

    explicit Board(const std::vector<std::vector<unsigned>> & data);

    std::size_t size() const;

    bool is_goal() const;

    unsigned hamming() const;

    unsigned manhattan() const;

    std::string to_string() const;

    bool is_solvable() const;
    friend bool operator == (const Board & lhs, const Board & rhs) {
        return lhs.table == rhs.table;
    }

    friend bool operator != (const Board & lhs, const Board & rhs) {
        return !(lhs == rhs);
    }

    friend bool operator > (const Board & lhs, const Board & rhs) {
        return lhs.table > rhs.table;
    }

    friend bool operator < (const Board & lhs, const Board & rhs) {
        return lhs.table < rhs.table;
    }

    // TODO:
    const std::vector<unsigned> & operator [] (std::size_t i) const;

    friend std::ostream & operator << (std::ostream & out, const Board & board)
    { return out << board.to_string(); }
};
