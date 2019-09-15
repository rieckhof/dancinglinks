#include "dancinglinks.h"

#include <algorithm>

namespace sudoku::dancinglinks {

std::shared_ptr<DancingLinks::ColumnObj> const& DancingLinks::get_root() const {
  return root;
};

std::shared_ptr<DancingLinks::ColumnObj>
DancingLinks::get_item_with_least_options() {
  std::shared_ptr<ColumnObj> result;
  auto last = root->right;
  while (last != root) {
    if (!result or result->size > last->size) {
      result = last;
    }
    last = last->right;
  }
  return result;
}

int32_t DancingLinks::get_header_size() const {
  return static_cast<int32_t>(header_size);
}

DancingLinks::DancingLinks() {  //: letters(number_of_leters){
  //    int begin (ascii_code_for_A);
  //    std::generate_n(letters.begin(), number_of_leters, [&begin](){
  //        return static_cast<char>(begin++);
  //    });
}

void DancingLinks::make_header(size_t size) {
  std::shared_ptr<ColumnObj> last = root;
  for (size_t i = 0; i < size; ++i) {
    std::shared_ptr<ColumnObj> nu = std::make_shared<ColumnObj>();
    insert(last, nu, i);
    if (i == size - 1) {
      nu->right = root;
      root->left = nu;
    } else {
      last = nu;
    }
    objs.push_back(nu);
  }
  header_size = objs.size();
}

std::shared_ptr<DancingLinks::ColumnObj> DancingLinks::get_object(
    size_t index) const {
  assert(index > 0);
  return objs.at(index - 1);
}

void DancingLinks::create_matrix(
    std::map<size_t, std::vector<uint16_t>> const& board) {
  make_header(board.at(0).size());
  for (auto& [index, data] : board) {
    make_row(data);
  }
  std::for_each(objs.begin(), objs.begin() + get_header_size(),
                [this](std::shared_ptr<ColumnObj>& header) {
                  header->size = count_elements_start_header(header);
                  auto last = get_last(header);
                  header->up = last;
                  last->down = header;
                });
  auto last_spacer = std::make_shared<ColumnObj>();
  last_spacer->up = get_object(get_last_spacer()->index + 1);
  last_spacer->top_spacer = spacer_counter--;
  last_spacer->index = objs.back()->index + 1;
  objs.push_back(last_spacer);
}

void DancingLinks::make_row(std::vector<uint16_t> const& data) {
  assert(data.size() == header_size);

  size_t last_index{objs.back()->index};
  std::shared_ptr<ColumnObj> spacer = std::make_shared<ColumnObj>();
  auto last_spacer = get_last_spacer();
  if (last_spacer and last_spacer->index != last_index) {
    spacer->up = get_object(last_spacer->index + 1);
  }
  spacer->top_spacer = spacer_counter--;
  spacer->index = ++last_index;
  objs.push_back(spacer);

  std::shared_ptr<ColumnObj> last_obj;
  for (size_t i = 0; i < data.size(); ++i) {
    if (data.at(i) == 1) {
      std::shared_ptr<ColumnObj>& header = objs.at(i);
      auto last = header->down == nullptr ? header : get_last(header);
      std::shared_ptr<ColumnObj> new_obj = std::make_shared<ColumnObj>();
      new_obj->top = header;
      new_obj->up = last;
      new_obj->index = ++last_index;
      last->down = new_obj;
      last_obj = new_obj;
      objs.push_back(new_obj);
    }
  }
  spacer->down = last_obj;
}

std::shared_ptr<DancingLinks::ColumnObj> DancingLinks::get_last_spacer() {
  auto result = std::find_if(
      objs.rbegin(), objs.rend(),
      [](std::shared_ptr<ColumnObj>& obj) { return obj->top_spacer <= 0; });
  return result == objs.rend() ? nullptr : *result;
}

std::shared_ptr<DancingLinks::ColumnObj> DancingLinks::get_start_spacer_from(
    std::shared_ptr<ColumnObj> const& obj) const {
  auto it = get_object(obj->index);
  while (it->top) {
    it = get_object(it->index + 1);
  }
  return get_object(it->up->index - 1);
}

void DancingLinks::print() const {
  std::shared_ptr<ColumnObj> iterator = root->right;
  while (iterator != root) {
    std::cout << "Header " << iterator->index << " " << iterator->size
              << std::endl;
    std::cout << "|" << std::endl;
    std::cout << "v" << std::endl;
    auto iterator_down = iterator->down;
    while (iterator_down and iterator_down != iterator) {
      std::cout << "Element " << iterator_down->index << std::endl;
      std::cout << "|" << std::endl;
      std::cout << "v" << std::endl;
      iterator_down = iterator_down->down;
    }
    iterator = iterator->right;
  }
}

std::shared_ptr<DancingLinks::ColumnObj> DancingLinks::get_last(
    std::shared_ptr<ColumnObj> iterator) {
  while (iterator) {
    if (!iterator->down) {
      return iterator;
    }
    iterator = iterator->down;
  }
  return iterator;
}

DancingLinks::~DancingLinks(){
    std::for_each(objs.begin(), objs.end(),
                 [](std::shared_ptr<ColumnObj>& obj){
        obj->down = nullptr;
        obj->up = nullptr;
        obj->right = nullptr;
        obj->left = nullptr;
        obj->top = nullptr;
    });
}

size_t DancingLinks::count_elements_start_header(
    std::shared_ptr<ColumnObj> iterator) {
  size_t result(0);
  while (iterator) {
    if (!iterator->down) {
      return result;
    }
    ++result;
    iterator = iterator->down;
  }
  return result;
}

void DancingLinks::insert(std::shared_ptr<ColumnObj>& last,
                          std::shared_ptr<ColumnObj>& nu,
                          size_t index) {
  last->right = nu;
  nu->left = last;
  //    nu->name = letters.at(index);
  nu->index = index + 1;
  nu->is_header = true;
}

int SudokuAdapter::get_box_index(size_t row_index,
                                   const size_t board_size,
                                   const ulong sqrt_from_size) {
  const size_t break_loop = board_size * board_size * board_size;
  assert(row_index < break_loop);

  size_t counter_box_cols(0);
  size_t counter_box_lines(0);
  int result(0);
  size_t start_multi(0);
  size_t end_multi(sqrt_from_size);
  unsigned long start(0);
  unsigned long end(0);
  while (end <= break_loop) {
    start = board_size * start_multi;
    end = (board_size * end_multi) - 1;
    //        std::cout << "start " << start << std::endl;
    //        std::cout << "end " << end<< std::endl;
    //        std::cout << "value " << result<< std::endl;

    if (start <= row_index and row_index <= end) {
      return result;
    }
    start_multi += sqrt_from_size;
    end_multi += sqrt_from_size;
    ++counter_box_cols;
    ++result;

    if (counter_box_cols % sqrt_from_size == 0) {
      ++counter_box_lines;
      if (counter_box_lines % sqrt_from_size != 0) {
        result -= sqrt_from_size;
      }
    }
  }
  return -1;
}

size_t SudokuAdapter::calculate_row_index(size_t index4board) const {
  size_t column_index(0);
  while (index4board >= matrix_size) {
    column_index++;
    index4board -= matrix_size;
  }
  return column_index;
}

size_t SudokuAdapter::calculate_column_index(size_t index4board) const {
  size_t column_index(0);
  while (index4board >= board_size) {
    column_index++;
    index4board -= board_size;
  }
  return column_index % board_size;
}

std::vector<SodokuMap> SudokuAdapter::create_sudoku_solved(
    std::vector<std::vector<size_t>> const& solutions) const {
  std::vector<SodokuMap> result;
  for (auto& solution_indexes : solutions) {
    std::vector<uint16_t> complete_solution;
    std::vector<uint16_t> solution_part;
    SodokuMap sol;
    for (size_t const& index4board : solution_indexes) {
      size_t row_index(calculate_row_index(index4board));
      size_t value((index4board % board_size) + 1);
      size_t column_index(calculate_column_index(index4board));

      std::string temp{std::to_string(row_index) +
                       std::to_string(column_index)};
      sol.insert({std::move(temp), value});
    }
    print_solution_to_console(sol);
    result.push_back(sol);
  }
  return result;
}

void SudokuAdapter::print_solution_to_console(SodokuMap const& solution) const {
  std::cout << "Solution:"
            << "\n";
  for (size_t row = 0; row < board_size; ++row) {
    if (row != 0 and constraints == 4 and (row % sqrt_from_size) == 0 ) {
      for (size_t col = 0; col < board_size * sqrt_from_size; ++col) {
        std::cout << "-";
      }
      std::cout << "\n";
    }
    for (size_t col = 0; col < board_size; ++col) {
      if (col != 0 and constraints == 4 and (col % sqrt_from_size) == 0) {
        std::cout << "|";
      }
      std::cout << solution.at(std::to_string(row) + std::to_string(col))
                << "  ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

uint16_t calculate_value_to_set(uint16_t const val_temp,
                                size_t const current_value) {
  uint16_t result{0};
  if (val_temp == 0) {
    result = 1;
  } else if (val_temp == current_value) {
    result = 1;
  }
  return result;
}

TheBoard SudokuAdapter::create_initial_board(SodokuMap const& map) {
  TheBoard b;

  if (sqrt_from_size * sqrt_from_size == board_size) {
    constraints++;
  }

  size_t current_value(0);
  size_t current_row(0);
  size_t current_col(0);
  size_t col_vs_row(0);
  size_t& colum_index_sodoku = current_row;
  size_t row_index_sodoku(0);
  for (size_t row_index = 0; row_index < std::pow(board_size, 3); ++row_index) {
    //        std::cout << "val " << current_value +1 << std::endl;
    //        std::cout << "col " << colum_index_sodoku % board_size <<
    //        std::endl; std::cout << "row " << row_index_sodoku << std::endl;
    //        std::cout << std::endl;
    auto val_temp = map.at(std::to_string(row_index_sodoku) +
                           std::to_string(colum_index_sodoku % board_size));
    uint16_t value_to_set = calculate_value_to_set(val_temp, current_value + 1);

    size_t constraint_counter(1);
    std::vector<uint16_t> row(matrix_size * constraints, 0);
    // constraint Row-Column
    row.at(current_row) = value_to_set;

    // constraint Row
    size_t index_offset(matrix_size * constraint_counter);
    size_t col_index{(index_offset + current_value + current_col)};
    row.at(col_index) = value_to_set;
    ++constraint_counter;

    // constraint Column
    size_t index_offset_c(matrix_size * constraint_counter);
    row.at(index_offset_c + col_vs_row) = value_to_set;

    if (constraints == 4) {
      ++constraint_counter;
      auto box = get_box_index(row_index, board_size, sqrt_from_size);
      assert(box != -1);
      size_t index_offset_const(matrix_size * constraint_counter);
      auto box_index = box * board_size;
      auto index_inside_box = col_vs_row % board_size;
      row.at(index_offset_const + box_index + index_inside_box) = value_to_set;
    }

    ++current_value;
    if (current_value == board_size) {
      current_value = 0;
      ++current_row;
    }

    ++col_vs_row;
    if (col_vs_row == matrix_size) {
      row_index_sodoku++;
      current_col += board_size;
      col_vs_row = 0;
    }

    b.insert({row_index, std::move(row)});
  }
  return b;
}

}  // namespace sudoku::dancinglinks
