#include "sudoku_matrix.h"
#include "utils.h"
#include <algorithm>
#include <functional>
#include <iostream>

namespace sudoku {
namespace datastruct {

bool operator<(std::pair<uint32_t, uint32_t> const &l_p,
               std::pair<uint32_t, uint32_t> const &r_p) {
  bool temp = l_p.first < r_p.first;
  return (l_p.second < r_p.second) && temp;
}

bool operator<=(std::pair<uint32_t, uint32_t> const &l_p,
                std::pair<uint32_t, uint32_t> const &r_p) {
  bool temp = l_p.first <= r_p.first;
  return (l_p.second <= r_p.second) && temp;
}

const Matrix::matrix_type &Matrix::get_internal() { return Matrix::m_matrix; }

void Matrix::init_to_empty() {
  for (uint32_t i = 0; i < m_size.first; ++i) {
    for (uint32_t j = 0; j < m_size.second; ++j) {
      m_matrix[{i, j}] = matrix_value_depth{};
    }
  }
}

Matrix::Matrix(std::size_t column_l, std::size_t with_l)
    : m_size{column_l, with_l} {}
Matrix::~Matrix() = default;
Matrix::Matrix(const Matrix &other) = default;
Matrix::Matrix(Matrix &&other) = default;

Matrix &Matrix::operator=(Matrix const &other) {
  Matrix::swap(other);
  return *this;
}

Matrix &Matrix::operator=(Matrix &&other) {
  swap(other);
  return *this;
}

void Matrix::swap(Matrix const &other) {
  Matrix temp(other);
  std::swap(m_matrix, temp.m_matrix);
  std::swap(m_size, temp.m_size);
}

void Matrix::set_value(std::pair<uint32_t, uint32_t> &&coordinate,
                       uint16_t value) {
  Matrix::operator[](std::move(coordinate)).insert(value);
}

Matrix::matrix_value_depth &Matrix::
operator[](std::pair<uint32_t, uint32_t> &&coordinate) {
  std::cout << "Coping" << std::to_string(coordinate.first)
            << std::to_string(coordinate.second) << std::endl;
  return operator[](coordinate);
}

Matrix::matrix_value_depth &Matrix::
operator[](std::pair<uint32_t, uint32_t> const &coordinate) {
  if (coordinate <= m_size) {
    return m_matrix[coordinate];
  }
  throw std::out_of_range("Coordinate " + std::to_string(coordinate.first) +
                          std::to_string(coordinate.second) +
                          " is out of range");
}

const Matrix::matrix_value_depth &
Matrix::get_value(std::pair<uint32_t, uint32_t> &&coordinate) {
  return operator[](std::move(coordinate));
}

const Matrix::matrix_size_t &Matrix::get_size() const { return Matrix::m_size; }

void Matrix::set_line(uint32_t line_i,
                      std::vector<matrix_value_depth> const &values) {
  if (values.size() == m_size.second) {
    for (auto index = values.begin(); index != values.end(); ++index) {
      uint32_t const col_index = std::distance(values.begin(), index);
      matrix_value_depth valToInsert = values.at(col_index);

      matrix_value_depth &temp = m_matrix[{col_index, line_i}];
      for (auto &value : valToInsert) {
        temp.insert(value);
      }
    }
  }
}

void Matrix::get_line(uint32_t const line_i,
                      std::vector<matrix_value_depth> &result) const {
  if (line_i <= m_size.second) {
    get_general(result, [&line_i](uint32_t counter, matrix_type const &matrix) {
      return matrix.at({counter, line_i});
    });
  }
}

template <typename F>
void Matrix::get_general(std::vector<matrix_value_depth> &result,
                         F func) const {
  uint32_t counter{0};
  while (counter < m_size.second) {
    result.push_back(func(counter, m_matrix));
    ++counter;
  }
}

void Matrix::set_column(uint32_t const column_i,
                        std::vector<matrix_value_depth> const &values) {
  if (values.size() == m_size.first) {
    for (auto index = values.begin(); index != values.end(); ++index) {
      uint32_t line_index = std::distance(values.begin(), index);
      matrix_value_depth valToInsert = values.at(line_index);

      matrix_value_depth &temp = m_matrix[{column_i, line_index}];
      for (auto &value : valToInsert) {
        temp.insert(value);
      }
    }
  }
}

void Matrix::get_column(uint32_t const columns_i,
                        std::vector<matrix_value_depth> &result) const {
  if (columns_i <= m_size.first) {
    get_general(result, [&](uint32_t counter, matrix_type const &matrix) {
      return matrix.at({columns_i, counter});
    });
  }
}

bool Matrix::operator==(Matrix const &other) const {
  for (auto col : utils::get_range(0ul, m_matrix.size())) {
    for (auto line : utils::get_range(0ul, m_matrix.size())) {
      auto local_val = m_matrix.at({col, line});
      auto other_val = other.m_matrix.at({col, line});
      if (local_val != other_val) {
        return false;
      }
    }
  }
  return true;
}

bool Matrix::operator!=(Matrix const &other) const {
  return !(m_matrix == other.m_matrix);
}

} // namespace datastruct
} // namespace sudoku
