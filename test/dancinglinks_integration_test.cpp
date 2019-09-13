#include <gtest/gtest.h>
#include "dancinglinks_solver.h"
#include "serializer.h"

using namespace sudoku::dancinglinks;

TEST(Solver, IntegrationTest99) {
  std::filesystem::path p = std::filesystem::current_path().append("sudoku99");
  SodokuMap map;
  size_t size = sudoku::serializer::deserialize(p, map);
  SudokuAdapter c(size);
  TheBoard b = c.create_initial_board(map);
  DancingLinksSolver dls(b);
  int level(0);
  dls.solve_algo_X(level);
  std::vector<SodokuMap> sol(dls.create_sudoku_solved(c));
}

TEST(Solver, IntegrationTest) {
    std::filesystem::path p = std::filesystem::current_path().append("sudoku");
    SodokuMap map;
    size_t size =sudoku::serializer::deserialize(p, map);
    SudokuAdapter c(size);
    TheBoard b = c.create_initial_board(map);
    DancingLinksSolver dls(b);
    int level(0);
    dls.solve_algo_X(level);
    std::vector<SodokuMap> sol(dls.create_sudoku_solved(c));
}
