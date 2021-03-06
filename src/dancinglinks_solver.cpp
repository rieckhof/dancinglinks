#include "dancinglinks_solver.h"

namespace sudoku::dancinglinks {

DancingLinksSolver::DancingLinksSolver(
    std::map<size_t, std::vector<uint16_t>> const& board) : dl(std::make_unique<DancingLinks>()) {
  dl->create_matrix(board);
}
DancingLinksSolver::DancingLinksSolver(
        DancingLinks& p_dl) : dl(std::make_unique<DancingLinks>(p_dl)){
}

std::vector<SodokuMap> DancingLinksSolver::create_sudoku_solved(
    SudokuAdapter const& b) const {
  auto const& temp = get_index_board();
  return b.create_sudoku_solved(temp);
}

void DancingLinksSolver::hide(std::shared_ptr<ColumnObj>& p) {
  auto q = dl->get_object(p->index + 1);
  while (p != q) {
    auto x = q->top;
    auto u = q->up;
    auto d = q->down;
    if (x == nullptr) {
      q = u;
    } else {
      u->down = d;
      d->up = u;
      --x->size;
      q = dl->get_object(q->index + 1);
    }
  }
}

void DancingLinksSolver::cover(std::shared_ptr<ColumnObj> const& header) {
  auto p = header->down;
  while (p != header) {
    hide(p);
    p = p->down;
  }
  auto& l = header->left;
  auto& r = header->right;
  l->right = r;
  r->left = l;
}

void DancingLinksSolver::unhide(std::shared_ptr<ColumnObj>& p) {
  auto q = dl->get_object(p->index - 1);
  while (q != p) {
    auto x = q->top;
    auto u = q->up;
    auto d = q->down;
    if (x == nullptr) {
      q = d;
    } else {
      u->down = q;
      d->up = q;
      ++x->size;
      q = dl->get_object(q->index - 1);
    }
  }
}

void DancingLinksSolver::uncover(std::shared_ptr<ColumnObj>& header) {
  auto& l = header->left;
  auto& r = header->right;
  l->right = header;
  r->left = header;
  auto p = header->up;
  while (p != header) {
    unhide(p);
    p = p->up;
  }
}

void DancingLinksSolver::create_solution(
    std::unordered_set<std::shared_ptr<ColumnObj>> const& bk_helper) {
  solution_t result;
  result.reserve(bk_helper.size());
  std::transform(bk_helper.begin(), bk_helper.end(), std::back_inserter(result),
                 [& dl = dl](std::shared_ptr<ColumnObj> const& x) {
                   return dl->get_start_spacer_from(x);
                 });
  solutions.push_back(result);
}

std::vector<std::vector<size_t>> DancingLinksSolver::get_index_board() const {
  std::vector<std::vector<size_t>> result;
  result.reserve(solutions.size());
  std::transform(solutions.begin(), solutions.end(), std::back_inserter(result),
                 [](solution_t const& sol) {
                   std::vector<size_t> temp;
                   temp.reserve(sol.size());
                   for (auto& obj : sol) {
                     temp.push_back(obj->top_spacer * -1);
                   }
                   return temp;
                 });
  return result;
}

void DancingLinksSolver::solve_algo_X(int& level) {
  if (dl->get_root()->right == dl->get_root()) {
    create_solution(backtracking_helper);
    if (level == 0) {
      return;
    } else {
      --level;
      return;
    }
  }
  auto item_i = dl->get_item_with_least_options();
  cover(item_i);
  auto x = item_i->down;
  while (x != item_i) {
    // Start X5
    backtracking_helper.insert(x);
    auto p = dl->get_object(x->index + 1);
    while (p != x) {
      auto header = p->top;
      if (header == nullptr) {
        p = p->up;
      } else {
        cover(header);
        p = dl->get_object(p->index + 1);
      }
    }
    // End X5
    solve_algo_X(++level);  // goto X2
    // Start X6
    backtracking_helper.erase(x);
    p = dl->get_object(x->index - 1);
    while (p != x) {
      auto j = p->top;
      if (j == nullptr) {
        p = p->down;
      } else {
        uncover(j);
        p = dl->get_object(p->index - 1);
      }
    }
    item_i = x->top;
    x = x->down;
    //          End X6
  }
  uncover(item_i);
}

}  // namespace sudoku::dancinglinks
