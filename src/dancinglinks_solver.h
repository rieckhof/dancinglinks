#ifndef DACINGLINK_SOLVER_H
#define DACINGLINK_SOLVER_H

#include <algorithm>
#include <memory>
#include <unordered_set>
#include "dancinglinks.h"

namespace sudoku::dancinglinks {

class DancingLinksSolver {
  using ColumnObj = DancingLinks::ColumnObj;
  std::unordered_set<std::shared_ptr<ColumnObj>> backtracking_helper;
  using solution_t = std::vector<std::shared_ptr<ColumnObj>>;
  std::vector<solution_t> solutions;

  void hide(std::shared_ptr<ColumnObj>& p);

  void unhide(std::shared_ptr<ColumnObj>& p);

 public:
  DancingLinks dl;
  DancingLinksSolver(std::map<size_t, std::vector<uint16_t>> const& board);

  std::vector<SodokuMap> create_sudoku_solved(SudokuAdapter const& b) const;

  void cover(std::shared_ptr<ColumnObj> const& header);

  void uncover(std::shared_ptr<ColumnObj>& header);

  void create_solution(
      std::unordered_set<std::shared_ptr<ColumnObj>> const& bk_helper);

  std::vector<std::vector<size_t>> get_index_board() const;

  void solve_algo_X(int& level);
};

}  // namespace sudoku::dancinglinks

#endif  // DACINGLINK_SOLVER_H
