#include "utils.h"

namespace sudoku {
namespace utils {

std::vector<uint16_t>
flatten(std::vector<datastruct::Board::matrix_value_depth> values) {
  std::vector<uint16_t> result;
  for (auto v : values) {
    if (v.size() == 1) {
      auto it = *v.begin();
      result.push_back(it);
    } else if (v.size() > 1) {
      throw std::logic_error("set cannot be flatten. Too many elements");
    }
  }
  return result;
}

std::vector<uint16_t> flatten(sudoku::datastruct::Matrix values) {
  std::vector<datastruct::Matrix::matrix_value_depth> result;
  for (uint16_t i = 0; i < 3; ++i) {
    values.get_line(i, result);
  }
  return flatten(result);
}

} // namespace utils
} // namespace sudoku
