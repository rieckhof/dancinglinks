#ifndef MATRIX_H
#define MATRIX_H

#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace sudoku {
namespace datastruct {

bool operator<(std::pair<uint32_t, uint32_t> const &l_p,
               std::pair<uint32_t, uint32_t> const &r_p);

bool operator<=(std::pair<uint32_t, uint32_t> const &l_p,
                std::pair<uint32_t, uint32_t> const &r_p);

struct pair_hash {
  template <typename T>
    std::size_t operator()(std::pair<T, T> const &p) const {
    std::size_t seed = 533;
    std::hash<T> hasher;
    seed ^= hasher(p.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hasher(p.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

struct Matrix {
  // pair {column, line}
  using matrix_value_depth = std::unordered_set<uint16_t>;
  using matrix_type = std::unordered_map<std::pair<uint32_t, uint32_t>,
                                         matrix_value_depth, pair_hash>;
  using matrix_size_t = std::pair<uint32_t, uint32_t>;

  explicit Matrix(std::size_t column_l = 3, std::size_t with_l = 3);
  ~Matrix();
  Matrix(const Matrix &copy);
  Matrix(Matrix &&copy);
  Matrix &operator=(Matrix const &other);
  Matrix &operator=(Matrix &&other);

  void set_line(uint32_t line_i, std::vector<matrix_value_depth> const &values);
  void get_line(uint32_t const line_i,
                std::vector<matrix_value_depth> &result) const;
  void set_column(uint32_t column_i,
                  std::vector<matrix_value_depth> const &values);
  void get_column(uint32_t const line_i,
                  std::vector<matrix_value_depth> &result) const;

  const matrix_type &get_internal();

  void init_to_empty();

  void set_value(std::pair<uint32_t, uint32_t> &&coordinate, uint16_t value);

  matrix_value_depth &
  operator[](std::pair<uint32_t, uint32_t> const &coordinate);
  matrix_value_depth &operator[](std::pair<uint32_t, uint32_t> &&coordinate);

  const matrix_value_depth &
  get_value(std::pair<uint32_t, uint32_t> &&coordinate);

  const matrix_size_t &get_size() const;

  bool operator==(Matrix const &) const;
  bool operator!=(Matrix const &) const;

private:
  template <typename F>
  void get_general(std::vector<matrix_value_depth> &result, F func) const;
  void swap(Matrix const &other);

  matrix_type m_matrix;
  matrix_size_t m_size;
};

} // namespace datastruct
} // namespace sudoku

#endif
