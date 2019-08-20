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

#ifndef BOARD_H
#define BOARD_H

#include <algorithm>
#include <assert.h>
#include <exception>
#include <iostream>
#include <vector>

#include "sudoku_matrix.h"

namespace sudoku {
namespace datastruct {

class Board {
  std::size_t m_size{9};
  std::vector<Matrix> board;
  std::vector<Matrix const *>
  get_matrixes_col(uint16_t sector, std::vector<Matrix> const &board) const {
    std::vector<Matrix const *> result;
    if (sector >= 0 && sector <= 2) {
      result.push_back(&board[0]);
      result.push_back(&board[3]);
      result.push_back(&board[6]);
    } else if (sector >= 3 && sector <= 5) {
      result.push_back(&board[1]);
      result.push_back(&board[4]);
      result.push_back(&board[7]);
    } else if (sector >= 6 && sector <= 8) {
      result.push_back(&board[2]);
      result.push_back(&board[5]);
      result.push_back(&board[8]);
    }
    return result;
  }

  std::vector<Matrix *> get_matrixes_col(uint16_t sector,
                                         std::vector<Matrix> &board) {
    std::vector<Matrix *> result;
    for (Matrix const *m :
         static_cast<Board const>(*this).get_matrixes_col(sector, board)) {
      result.push_back(const_cast<Matrix *>(m));
    }
    return result;
  }

  std::vector<Matrix const *>
  get_matrixes_lines(uint16_t sector, std::vector<Matrix> const &board) const {
    std::vector<Matrix const *> result;
    if (sector >= 0 && sector <= 2) {
      result.push_back(&board[0]);
      result.push_back(&board[1]);
      result.push_back(&board[2]);
    } else if (sector >= 3 && sector <= 5) {
      result.push_back(&board[3]);
      result.push_back(&board[4]);
      result.push_back(&board[5]);
    } else if (sector >= 6 && sector <= 8) {
      result.push_back(&board[6]);
      result.push_back(&board[7]);
      result.push_back(&board[8]);
    }
    return result;
  }

  std::vector<Matrix *> get_matrixes_lines(uint16_t sector,
                                           std::vector<Matrix> &board) {
    std::vector<Matrix *> result;
    for (Matrix const *m :
         static_cast<Board const>(*this).get_matrixes_lines(sector, board)) {
      result.push_back(const_cast<Matrix *>(m));
    }
    return result;
  }

public:
  using matrix_value_depth = Matrix::matrix_value_depth;
  Board() : m_size{9}, board(m_size) {}

  Matrix get_sector(uint16_t const sector) {
    assert(board.size() > sector);
    return board.at(sector);
  }

  Matrix get_sector_of_field(std::pair<uint16_t, uint16_t> field_coord) {
    assert(field_coord.first < m_size);
    assert(field_coord.second < m_size);

    uint16_t sector = 3 * (field_coord.first / 3) + field_coord.second / 3;
    return get_sector(sector);
  }

  void init_all_empty() {
    for (auto &matrix : board)
      matrix.init_to_empty();
  }

  const std::vector<Matrix> &get_board() { return board; }

  matrix_value_depth get_field(uint16_t col, uint16_t line) {
    assert(col < 9);
    assert(line < 9);

    std::vector<matrix_value_depth> result;
    get_column(col, result);
    return result.at(line);
  }

  void set_field(uint16_t col, uint16_t line, uint16_t value) {
    assert(col < 9);
    assert(line < 9);

    std::vector<Matrix *> cols(get_matrixes_col(col, board));
    std::vector<Matrix *> lines(get_matrixes_lines(line, board));

    std::vector<Matrix *> result;
    std::set_intersection(cols.begin(), cols.end(), lines.begin(), lines.end(),
                          std::back_inserter(result));

    assert(result.size() == 1);
    Matrix *matrix = *result.begin();
    matrix->set_value({col % 3, line % 3}, value);
  }

  void get_column(uint32_t col_i,
                  std::vector<matrix_value_depth> &result) const {
    for (Matrix const *matrix : get_matrixes_col(col_i, board)) {
      matrix->get_column(col_i % 3, result);
    }
  }

  void get_line(uint32_t line_i,
                std::vector<matrix_value_depth> &result) const {
    for (Matrix const *matrix : get_matrixes_lines(line_i, board)) {
      matrix->get_line(line_i % 3, result);
    }
  }

  template <typename F>
  void set(std::vector<matrix_value_depth> const &values,
           std::vector<Matrix *> const &matrixes, F func) {
    if (values.size() != m_size) {
      throw std::out_of_range("index out of range");
    }
    std::vector<matrix_value_depth> result;
    uint32_t index{0};
    for (Matrix *matrix : matrixes) {
      auto be = values.begin() + index;
      auto end = be + matrix->get_size().second;
      result.assign(be, end);
      func(matrix, result);
      index += matrix->get_size().second;
    }
  }

  void set_column(uint32_t col_i, std::vector<matrix_value_depth> const &col) {
    set(col, get_matrixes_col(col_i, board),
        [&col_i](Matrix *matrix, std::vector<matrix_value_depth> result) {
          matrix->set_column(col_i % 3, result);
        });
  }

  void set_line(uint32_t line_i,
                std::vector<matrix_value_depth> const &values) {
    set(values, get_matrixes_lines(line_i, board),
        [&line_i](Matrix *matrix, std::vector<matrix_value_depth> result) {
          matrix->set_line(line_i % 3, result);
        });
  }
};

std::ostream &operator<<(std::ostream &stream, Board &b);

} // namespace datastruct
} // namespace sudoku

#endif // BOARD_H
