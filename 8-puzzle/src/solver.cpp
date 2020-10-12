#include "solver.h"
#include <set>
#include <map>
#include <vector>
#include <algorithm>

bool check_cell(unsigned x, unsigned y, size_t size) {
    return (x < size && y < size);
}

Solver::Solver(const Board &board) {
    Board cur_table = board;
    if (board.is_goal()) {
        m_moves.push_back(cur_table);
    } else if (cur_table.is_solvable()) {
        std::map<Board, Board> parent;
        std::set<std::pair<unsigned, Board>> priority;
        std::map<Board, unsigned> cost;
        cost[cur_table] = 0;
        while (!cur_table.is_goal()) {
            unsigned x = 0, y = 0;
            for (size_t i = 0; i < cur_table.size(); ++i) {
                for (size_t j = 0; j < cur_table.size(); ++j) {
                    if (cur_table[i][j] == 0) {
                        x = i;
                        y = j;
                    }
                }
            }

            Board prev_table = cur_table;
            unsigned cur_cost = cost[cur_table] + 1;

            for (auto & i : direction) {
                unsigned cur_x = x + i.first;
                unsigned cur_y = y + i.second;
                if (check_cell(cur_x, cur_y, cur_table.size())) {
                    cur_table.swap_cells(cur_x, cur_y, x, y);
                    if (cost.count(cur_table) == 0 || cost[cur_table] > cur_cost) {
                        parent[cur_table] = prev_table;
                        cost[cur_table] = cur_cost;
                        priority.insert({prev_table.hamming() + prev_table.manhattan() + cur_cost, cur_table});
                    }
                    cur_table.swap_cells(cur_x, cur_y, x, y);
                }
            }
            cur_table = priority.begin()->second;
            priority.erase(priority.begin());
        }

        cur_table = Board::create_goal(cur_table.size());
        while (cur_table != board) {
            m_moves.push_back(cur_table);
            cur_table = parent[cur_table];
        }
        m_moves.push_back(cur_table);
        std::reverse(m_moves.begin(), m_moves.end());
    }
}

std::size_t Solver::moves() const {
    if (m_moves.empty()) {
        return 0;
    } else {
        return m_moves.size() - 1;
    }
}

