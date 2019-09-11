#include "solver.h"
#include "utils.h"
#include <math.h>
#include <exception>

namespace sudoku {
namespace solver {

using field_value = datastruct::Board::matrix_value_depth;

int TheBoardComplex::get_box_index(size_t row_index, const size_t board_size, const ulong sqrt_from_size){
    const size_t break_loop = board_size * board_size * board_size;
    assert(row_index < break_loop);

    size_t counter_box_cols(0);
    size_t counter_box_lines(0);
    int result(0);
    size_t start_multi(0);
    size_t end_multi(sqrt_from_size);
    unsigned long start(0);
    unsigned long end(0);
    while(end <= break_loop){
        start = board_size * start_multi;
        end = (board_size * end_multi) - 1;
//        std::cout << "start " << start << std::endl;
//        std::cout << "end " << end<< std::endl;
//        std::cout << "value " << result<< std::endl;

        if(start <= row_index and row_index <= end){
            return result;
        }
        start_multi += sqrt_from_size;
        end_multi += sqrt_from_size;
        ++counter_box_cols;
        ++result;

        if(counter_box_cols % sqrt_from_size == 0){
            ++counter_box_lines;
            if(counter_box_lines % sqrt_from_size != 0){
                result -= sqrt_from_size;
            }
        }
    }
    return -1;
}

TheBoard TheBoardComplex::create_initial_board(const size_t board_size){
    TheBoard b;
    size_t constraints (3);
    std::unordered_map<std::string,int> map;
    const ulong sqrt_from_size{static_cast<ulong>(
                    std::lround( std::sqrt( board_size )))};
    if( sqrt_from_size * sqrt_from_size == board_size ){
        constraints++;
    }

    size_t current_value(0);
    size_t current_row(0);
    size_t current_col(0);
    size_t col_vs_row(0);
    const size_t matrix_size{board_size * board_size};

    for(size_t row_index = 0; row_index < std::pow(board_size,3); ++row_index) {
        size_t constraint_counter(1);
        std::vector<uint16_t> row (matrix_size * constraints, 0);
        //constraint Row-Column
        row.at(current_row) = 1;

        //constraint Row
        size_t index_offset(matrix_size * constraint_counter);
        size_t col_index{(index_offset + current_value + current_col)};
        row.at(col_index) = 1;
        ++constraint_counter;

        //constraint Column
        size_t index_offset_c(matrix_size * constraint_counter);
        row.at(index_offset_c + col_vs_row) = 1;

        if(constraints == 4) {
            ++constraint_counter;
            auto box = get_box_index(row_index,board_size,sqrt_from_size);
            assert(box != -1);
            size_t index_offset_const(matrix_size * constraint_counter);
            auto box_index = box * board_size;
            auto index_inside_box = col_vs_row % board_size;
            row.at(index_offset_const + box_index + index_inside_box) = 1;
        }

        ++current_value;
        if(current_value == board_size){
            current_value = 0;
            ++current_row;
        }

        ++col_vs_row;
        if(col_vs_row == matrix_size){
             current_col += board_size;
             col_vs_row = 0;
        }
        std::
        b.insert({row_index, std::move(row)});
    }
    return b;
}

size_t TheBoardComplex::adjust_row_column_constraint(size_t const col, size_t const line, uint16_t const value, size_t const board_size){

}


bool TheBoardComplex::remove_value(TheBoard& b, size_t const col, size_t const line,
                  uint16_t const value, size_t const board_size)
{
    size_t l_col(0);
    size_t l_line(0);
    size_t l_valu(0);
    for(auto& line :b){


    }
}

datastruct::Board SudokuSolver::solve(datastruct::Board &b) {
  for (auto index : utils::range) {
    std::vector<datastruct::Board::matrix_value_depth> col_values;
    b.get_column(index, col_values);
    bool col_check = sanity_check(utils::flatten(col_values));
    assert(col_check);

    std::vector<datastruct::Board::matrix_value_depth> line_values;
    b.get_line(index, line_values);
    bool line_check = sanity_check(utils::flatten(line_values));
    assert(line_check);

    std::vector<uint16_t> sector_flat{utils::flatten(b.get_sector(index))};
    bool sector_check = sanity_check(sector_flat);
    assert(sector_check);
  }
  check_values(b);
  return b;
}

bool SudokuSolver::sanity_check(std::vector<uint16_t> const &values_flat) {
  std::unordered_set<uint16_t> to_test{};
  for (auto val_temp : values_flat) {
    to_test.insert(val_temp);
  }

  return values_flat.size() == to_test.size();
}

void SudokuSolver::check_values(datastruct::Board &b) {
  // deep first search with back tracking
  std::pair<uint16_t, uint16_t> coordinates{0, 0};
  field_value field = b.get_field(coordinates.first, coordinates.second);

  std::vector<field_value> column;
  b.get_column(coordinates.first, column);

  std::vector<field_value> line;
  b.get_line(coordinates.second, line);

  auto value = b.get_sector_of_field(coordinates);
}
//return true if there are no columns, otherwise false
bool SudokuSolver::no_columns(TheBoard &b) {
    auto result = std::find_if(b.begin(),b.end(),
                 [](auto& el){
                    return !el.second.empty();
                });
    return result == b.end();
}

std::vector<uint16_t> get_column(TheBoard const&b, size_t column_index) {
    std::vector<uint16_t> result;
    std::transform(b.begin(),b.end(), std::back_inserter(result),
               [column_index](auto& vec) -> uint16_t {
                      return vec.second.at(column_index);
                   });
    return result;
}

std::pair<std::vector<uint16_t>,size_t> get_column_with_least_ones(TheBoard const&b) {
    std::vector<uint16_t> result;
    long min_ones{std::numeric_limits<long>::max()};
    size_t result_column(0);
    for (size_t i = 0; i < b.size(); ++i) {
        std::vector<uint16_t> temp { get_column(b,i) };
        long ones = std::count_if(temp.begin(),temp.end(),
                [](auto element){
                    return element == 1;
        });
        if (ones < min_ones){
            result_column = i;
            result.assign(temp.begin(),temp.end());
            min_ones = ones;
        }
    }

    return std::make_pair(result,result_column);
}

std::vector<size_t> get_indexes_of_ones(std::vector<uint16_t> const& determ_select_col){
    std::vector<size_t> result;
    for(size_t i = 0; i <determ_select_col.size() ; ++i){
        if(determ_select_col[i] == 1){
            result.push_back(i);
        }
    }
    return result;
}

std::vector<size_t> get_indexes_of_ones_for_col(size_t const& determ_select_col, TheBoard const& b)
{
    std::vector<size_t> result;
    //Possible performance boost by counting first and reserving the mem
    //So it does not realocate
    for(auto& [index, vec]  : b){
        if(vec.at(determ_select_col) == 1){
              result.push_back(index);
        }
    }
    return result;
}

TheBoard SudokuSolver::step6(std::vector<size_t> const& col_index_with_ones, TheBoard b)
{
    //delete rows
    for(auto it = b.begin(); it != b.end(); ) {
        auto& vec = it->second;
        auto contains = std::find_if(col_index_with_ones.begin(),
                                     col_index_with_ones.end(),
                                     [&vec](size_t index) {
                                        return vec.at(index) == 1;
                                });
        if(contains != col_index_with_ones.end()) {
            b.erase(it++);
        } else {
            ++it;
        }
    }

   //delete columns
     for(auto& [index, vec] : b){
       size_t counter_erases(0);
       for(size_t index : col_index_with_ones) {
             vec.erase(vec.begin() + (index - counter_erases));
             ++counter_erases;
        }
    }
    return b;
}

bool SudokuSolver::solve_algorithm_x(TheBoard&& b, TheSolution& solution)
{
    if(no_columns(b)) {
        return true;
    } else {
        auto const& [select_col, selected_col_index] = get_column_with_least_ones(b);
        if (select_col.empty()) return false;
        std::vector<size_t> row_index_with_ones{
                                get_indexes_of_ones_for_col(selected_col_index, b)};
        for(size_t index_row  : row_index_with_ones) {
            solution.insert(index_row);
            std::vector<size_t> col_index_with_ones{
                                    get_indexes_of_ones(b[index_row])}; //step 5
            bool temp_result( solve_algorithm_x(
                                  step6(col_index_with_ones, b), solution));
            if (!temp_result) {
                solution.erase(index_row);
            } else {
                return true;
            }
        }
        return false;
    }
}

} // namespace solver
} // namespace sudoku
