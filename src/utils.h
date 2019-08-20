#ifndef UTILS_H
#define UTILS_H
#include "board.h"
#include <algorithm>
#include <vector>

namespace sudoku {
namespace utils {

static std::vector<uint16_t> const range{0, 1, 2, 3, 4, 5, 6, 7, 8};

std::vector<uint16_t>
flatten(std::vector<datastruct::Board::matrix_value_depth> values);

std::vector<uint16_t> flatten(sudoku::datastruct::Matrix values);

template <typename T> std::vector<T> get_range(T start, T end) {
  assert(end >= start);
  std::vector<T> result((end - start) + 1);
  std::generate(result.begin(), result.end(), [&start]() { return start++; });
  return result;
}

} // namespace utils
} // namespace sudoku

#endif // UTILS_H
