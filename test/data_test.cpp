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

#include "sudoku_matrix.h"
#include <gtest/gtest.h>

using namespace sudoku::datastruct;

TEST(MatrixTest, Intialization) {
  Matrix m;

  EXPECT_EQ(m.get_size().first, 3);
  EXPECT_EQ(m.get_size().second, 3);
}

TEST(MatrixTest, CopySize) {
  Matrix m_big(4, 4);
  Matrix m_small;

  EXPECT_GT(m_big.get_size().first, m_small.get_size().first);
  m_small = m_big;
  EXPECT_EQ(m_big.get_size().first, m_small.get_size().first);
  EXPECT_EQ(m_big.get_size().first, 4);
  EXPECT_EQ(m_small.get_size().first, 4);

  Matrix matrix_copy{m_small};
  EXPECT_EQ(matrix_copy.get_size().first, 4);
}

using matrix_value_depth = sudoku::datastruct::Matrix::matrix_value_depth;

TEST(MatrixTest, CopyValues) {
  Matrix m_big(4, 4);
  m_big.init_to_empty();
  int c = 0;

  for (auto const &val : m_big.get_internal()) {
    EXPECT_EQ(m_big[val.first], matrix_value_depth(0));
    ++c;
  }

  EXPECT_EQ(c, 16);
  matrix_value_depth expected{3};
  m_big[{1, 2}] = expected;
  matrix_value_depth temp = m_big[{1, 2}];
  EXPECT_EQ(temp, expected);
}

std::vector<matrix_value_depth> fill_with(uint16_t start, uint16_t end) {
  std::vector<matrix_value_depth> result;
  while (start <= end) {
    uint16_t temp = start;
    result.push_back(matrix_value_depth{temp});
    ++start;
  }
  return result;
}

struct GetterSetterTest : public ::testing::Test {
  Matrix m_big{};
  std::vector<matrix_value_depth> expected{fill_with(1, 3)};
  std::vector<matrix_value_depth> result;
  virtual void SetUp() { m_big.init_to_empty(); }

  void check(std::vector<matrix_value_depth> const &result) {
    EXPECT_EQ(expected.size(), result.size());
    EXPECT_EQ(expected, result);
  }
};

TEST_F(GetterSetterTest, SetColumn) {
  m_big.set_column(2, expected);
  m_big.get_column(2, result);
  check(result);
}

TEST_F(GetterSetterTest, SetLine) {
  m_big.set_line(0, expected);
  m_big.get_line(0, result);
  check(result);
}

TEST(MatrixTest, Set2Line) {
  Matrix m_big(4, 4);
  m_big.init_to_empty();
  std::vector<matrix_value_depth> expected{fill_with(1, 4)};
  std::vector<matrix_value_depth> expected2{fill_with(5, 8)};
  m_big.set_line(0, expected);
  m_big.set_line(1, expected2);

  matrix_value_depth val = m_big[{0, 1}];
  matrix_value_depth val_z = m_big[{0, 2}];
  matrix_value_depth f{5};
  EXPECT_EQ(val, f);
  EXPECT_EQ(val_z, matrix_value_depth{});

  std::vector<matrix_value_depth> result{};
  std::vector<matrix_value_depth> result2{};
  m_big.get_line(1, result2);
  m_big.get_line(0, result);
  EXPECT_EQ(expected2.size(), result2.size());
  EXPECT_EQ(expected.size(), result.size());
  EXPECT_EQ(expected, result);
  EXPECT_EQ(expected2, result2);
}
