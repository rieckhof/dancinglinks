#include "serializer.h"
#include <algorithm>
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace sudoku {
namespace serializer {

/**
 *0 1 3
 *2 0 0
 *3 1 0
 */

void deserialize(std::filesystem::path const& p,
                 std::unordered_map<std::string, u_int16_t>& map) {
  assert(std::filesystem::exists(p));

  std::ifstream infile(p.c_str());
  std::string line;

  static constexpr uint16_t ASCCI_ZERO(48);
  size_t col_index(0);
  size_t check_all_same_size(0);

  while (std::getline(infile, line)) {
    auto it = std::remove_if(line.begin(), line.end(),
                             [](char a) { return std::isspace(a); });
    line.erase(it, line.end());

    if (check_all_same_size == 0) {
      check_all_same_size = line.size();
    } else if (check_all_same_size != line.size()) {
      std::cout << "current size " << line.size();
      std::cout << "previous " << check_all_same_size;
      throw std::logic_error("All lines in sudoku have to be same length");
    }

    size_t line_index(0);
    for (auto value : line) {
      std::string temp{std::to_string(col_index) + std::to_string(line_index)};
      map.insert({std::move(temp), value - ASCCI_ZERO});
      line_index++;
    }
    col_index++;
  }
  if (col_index != check_all_same_size) {
    std::cout << "cols " << col_index << "\n";
    std::cout << "check_all_same_size " << check_all_same_size << "\n";
    throw std::logic_error("Columns and rows have to be the same length");
  }
}

}  // namespace serializer
}  // namespace sudoku
