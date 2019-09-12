/*
 * Copyright 2018 federico <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "board.h"
#include <gtest/gtest.h>

using namespace sudoku::datastruct;
using matrix_value_depth = Matrix::matrix_value_depth;

TEST(BoardTest, InitBoard) {
  Board b;
  b.init_all_empty();
  std::vector<Matrix> internal_b = b.get_board();
  EXPECT_EQ(internal_b.size(), 9);
  matrix_value_depth expected(0);
  int c = 0;

  for (auto& m : internal_b) {
    ++c;
    matrix_value_depth temp = m[{1, 2}];
    EXPECT_EQ(temp, expected);
  }
  EXPECT_EQ(c, 9);
}

using matrix_value_depth = sudoku::datastruct::Matrix::matrix_value_depth;

std::vector<matrix_value_depth> fill_values() {
  std::vector<matrix_value_depth> result(9);
  uint16_t counter{1};
  for (matrix_value_depth& r : result) {
    uint16_t to_insert = counter;
    r.insert(to_insert);
    ++counter;
  }
  return result;
}

std::vector<matrix_value_depth> fill_values(std::vector<uint16_t>&& input) {
  std::vector<matrix_value_depth> result(9);
  assert(input.size() == result.size());

  uint16_t index{0};
  for (matrix_value_depth& r : result) {
    r.insert(input.at(index));
    ++index;
  }
  return result;
}

TEST(BoardTest, setColumn) {
  Board b;
  b.init_all_empty();
  std::vector<matrix_value_depth> expected = fill_values();
  b.set_column(1, expected);
  std::vector<matrix_value_depth> result;
  b.get_column(1, result);

  EXPECT_EQ(expected, result);
}
TEST(BoardTest, setColumn4) {
  Board b;
  b.init_all_empty();
  std::vector<matrix_value_depth> expected = fill_values();
  b.set_column(4, expected);
  std::vector<matrix_value_depth> result;
  b.get_column(4, result);

  EXPECT_EQ(result.size(), 9);
  EXPECT_EQ(expected, result);
}

TEST(BoardTest, setLine) {
  Board b;
  b.init_all_empty();
  std::vector<matrix_value_depth> expected = fill_values();
  b.set_line(3, expected);

  std::vector<matrix_value_depth> result;
  b.get_line(3, result);
  EXPECT_EQ(expected, result);
  std::vector<matrix_value_depth> expected_zero(9);

  for (int i = 0; i < 9; ++i) {
    result.clear();
    if (i != 3) {
      b.get_line(i, result);
      EXPECT_EQ(expected_zero, result) << "for line " << i << "\n";
    }
  }
}

matrix_value_depth get_diference(matrix_value_depth& expected,
                                 matrix_value_depth& real) {
  matrix_value_depth result;
  std::set_difference(expected.begin(), expected.end(), real.begin(),
                      real.end(), std::inserter(result, result.begin()));
  return result;
}

TEST(BoardTest, setLineColumnAnGetLines) {
  Board b;
  b.init_all_empty();
  std::vector<matrix_value_depth> expected_line =
      fill_values(std::vector<uint16_t>{1, 2, 3, 5, 4, 6, 7, 8, 9});
  std::vector<matrix_value_depth> expected_col =
      fill_values(std::vector<uint16_t>{9, 2, 3, 4, 1, 5, 6, 7, 8});
  b.set_line(4, expected_line);
  b.set_column(3, expected_col);

  std::vector<matrix_value_depth> result;
  b.get_line(4, result);

  matrix_value_depth test{5, 1};
  EXPECT_EQ(b.get_field(3, 4), test);

  // another way to campare
  matrix_value_depth to_compare = b.get_field(3, 4);
  EXPECT_EQ(test, to_compare);

  std::vector<matrix_value_depth> expected_line_2 =
      fill_values(std::vector<uint16_t>{1, 2, 3, 1, 4, 6, 7, 8, 9});
  expected_line_2.at(3).insert(5);
  EXPECT_EQ(expected_line_2, result);
}

TEST(BoardTest, getSector) {
  Board b;
  b.init_all_empty();
  std::vector<matrix_value_depth> expected_col =
      fill_values(std::vector<uint16_t>{9, 2, 3, 4, 1, 5, 6, 7, 8});
  b.set_column(3, expected_col);

  Matrix m = b.get_sector(4);
  std::vector<matrix_value_depth> res;
  m.get_column(0, res);

  auto begin = expected_col.begin();
  std::vector<matrix_value_depth> expected(begin + 3, begin + 6);
  EXPECT_EQ(res, expected);

  Matrix ma = b.get_sector_of_field(std::pair<uint16_t, uint16_t>{3, 4});
  res.clear();
  m.get_column(0, res);
  EXPECT_EQ(res, expected);
  EXPECT_EQ(m, ma);
}

TEST(BoardTest, setField) {
  Board b;
  b.init_all_empty();

  b.set_field(3, 4, 1);
  matrix_value_depth result_1{1};
  EXPECT_EQ(b.get_field(3, 4), result_1);

  std::vector<matrix_value_depth> expected_zero(9);
  std::vector<matrix_value_depth> result;
  for (int i = 0; i < 9; ++i) {
    result.clear();
    if (i != 4) {
      b.get_line(i, result);
      EXPECT_EQ(expected_zero, result) << "for line " << i << "\n";
    }
  }
}
