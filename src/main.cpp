#include <chrono>
#include "dancinglinks_solver.h"
#include "serializer.h"
using namespace sudoku::dancinglinks;

int main(int argc, char** argv) {
  std::cout << "Starting " << argv[0] << "\n";
  auto start = std::chrono::high_resolution_clock::now().time_since_epoch();
  if (argc < 2) {
    std::cout << "  Please provide a file in the same folder as the executable"
              << "\n";
    return 0;
  }
  std::filesystem::path p = std::filesystem::current_path().append(argv[1]);
  SodokuMap map;

  std::cout << "  Reading " << argv[1] << "\n";
  sudoku::serializer::deserialize(p, map);
  SudokuAdapter c(9);
  TheBoard b = c.create_initial_board(map);
  std::cout << "  Starting Solver "
            << "\n";

  DancingLinksSolver dls(b);
  int level(0);
  dls.solve_algo_X(level);
  std::vector<SodokuMap> sol(dls.create_sudoku_solved(c));
  if (sol.empty())
    std::cout << "  No solution found"
              << "\n";
  auto end = std::chrono::high_resolution_clock::now().time_since_epoch();
  std::chrono::duration<double, std::milli> total_time = end - start;
  std::cout << "Total calculation time: " << total_time.count() << " ms"
            << "\n";
  return 0;
}
