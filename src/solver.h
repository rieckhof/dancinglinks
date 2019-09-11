#ifndef SOLVER_H
#define SOLVER_H
#include "board.h"
#include <map>

namespace sudoku::solver {
//TODO change to pair<int,std::vector<uint16_t>>


using TheBoard = std::map<size_t,std::vector<uint16_t>>;
using TheSolution = std::unordered_set<size_t>;

std::pair<std::vector<uint16_t>, size_t> get_column_with_least_ones(TheBoard const& b);
std::vector<uint16_t> get_column(TheBoard const& b, size_t column_index);
std::vector<size_t> get_indexes_of_ones(std::vector<uint16_t> const& determ_select_col);
std::vector<size_t> get_indexes_of_ones_for_col(size_t const& determ_select_col, TheBoard const& b);

TheBoard create_initial_board(size_t board_size);
int get_box_index(size_t row_index, const size_t board_size, const ulong sqrt_from_size);

bool remove_value(TheBoard& b, size_t col, size_t line, uint16_t value);


class SudokuSolver {
public:
  datastruct::Board solve(datastruct::Board &b);
  bool sanity_check(std::vector<uint16_t> const &values_flat);
  void check_values(datastruct::Board &b);

  bool solve_algorithm_x(TheBoard&& board,TheSolution& solution);
  bool no_columns(TheBoard &b);
  TheBoard step6(std::vector<size_t> const& col_index_with_ones, TheBoard b);
};

} // namespace solver
#endif // SOLVER_H
