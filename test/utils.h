#ifndef UTILS_H
#define UTILS_H

#include <gtest/gtest.h>
#include "dancinglinks.h"

namespace sudoku::dancinglinks {

void iterate_down(std::shared_ptr<DancingLinks::ColumnObj>& it) {
  it = it->down;
}

void iterate_up(std::shared_ptr<DancingLinks::ColumnObj>& it) {
  it = it->up;
}

std::vector<size_t> create_test_results(
    std::shared_ptr<DancingLinks::ColumnObj> iterator_down,
    std::shared_ptr<DancingLinks::ColumnObj> const& iterator,
    std::function<void(std::shared_ptr<DancingLinks::ColumnObj>&)> f) {
  std::vector<size_t> result;
  while (iterator_down and iterator_down != iterator) {
    result.push_back(iterator_down->index);
    f(iterator_down);
  }
  return result;
}

void check_all(std::shared_ptr<DancingLinks::ColumnObj> const& iterator,
               std::vector<size_t> const& expected_values) {
  std::vector<size_t> result;
  result.push_back(iterator->index);
  auto temp = create_test_results(iterator->down, iterator, iterate_down);
  result.insert(result.end(), temp.begin(), temp.end());
  EXPECT_EQ(result, expected_values);
}

void check_all_up(std::shared_ptr<DancingLinks::ColumnObj> const& iterator,
                  std::vector<size_t> const& expected_values) {
  std::vector<size_t> result;
  result.push_back(iterator->index);
  auto temp = create_test_results(iterator->up, iterator, iterate_up);
  result.insert(result.end(), temp.begin(), temp.end());
  EXPECT_EQ(result, expected_values);
}

void check_all_directions(
    DancingLinks const& dl,
    std::vector<std::vector<size_t>>& all_expected_values) {
  auto iterator = dl.get_root()->right;
  for (auto& expected : all_expected_values) {
    check_all(iterator, expected);
    iterator = iterator->right;
  }
  EXPECT_EQ(iterator, dl.get_root());

  iterator = dl.get_root()->left;
  std::reverse(all_expected_values.begin(), all_expected_values.end());
  for (auto& expected : all_expected_values) {
    check_all(iterator, expected);
    iterator = iterator->left;
  }
  EXPECT_EQ(iterator, dl.get_root());

  std::reverse(all_expected_values.begin(), all_expected_values.end());
  iterator = dl.get_root()->right;
  for (auto& expected : all_expected_values) {
    std::reverse(expected.begin() + 1, expected.end());
  }
  for (auto& expected : all_expected_values) {
    check_all_up(iterator, expected);
    iterator = iterator->right;
  }
  EXPECT_EQ(iterator, dl.get_root());
}

}  // namespace sudoku::dancinglinks
#endif  // UTILS_H
