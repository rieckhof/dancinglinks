#ifndef DANCINGLINKS_H
#define DANCINGLINKS_H

#include <memory>
#include <vector>
#include <vector>
#include <iostream>
#include <cassert>
#include <map>
#include "serializer.h"
#include "solver.h"

namespace sudoku::dancinglinks {

using TheBoard = std::map<size_t,std::vector<uint16_t>>;
using SodokuMap = std::unordered_map<std::string, u_int16_t>;

class TheBoardComplex{
public:
    TheBoard create_initial_board(size_t board_size, SodokuMap& map);
    int get_box_index(size_t row_index, const size_t board_size, const ulong sqrt_from_size);
};


class DancingLinks
{
public:

    struct ColumnObj{
        size_t size {0};
        std::string name {""};
        size_t index {0};
        int32_t top_spacer {1};
        bool is_header {false};
        std::shared_ptr<ColumnObj> right;
        std::shared_ptr<ColumnObj> left;
        std::shared_ptr<ColumnObj> down;
        std::shared_ptr<ColumnObj> up;
        std::shared_ptr<ColumnObj> top;
    };

    std::shared_ptr<ColumnObj>const& get_root() const;

    std::shared_ptr<ColumnObj> get_item_with_least_options();

    int32_t get_header_size() const;

    DancingLinks();

    void make_header(size_t size);

    std::shared_ptr<ColumnObj> get_object(size_t index) const;

    void create_matrix(TheBoard const& board);

    void make_row(std::vector<uint16_t>const& data);

    std::shared_ptr<ColumnObj> get_last_spacer();

    std::shared_ptr<ColumnObj> get_start_spacer_from(std::shared_ptr<ColumnObj>const& obj) const;

    void print() const;

private:
    constexpr static int number_of_leters{35};
    constexpr static int ascii_code_for_A{65};
    std::shared_ptr<ColumnObj> root {std::make_shared<ColumnObj>()};
    std::vector<char> letters;
    size_t header_size{0}; // no root
    int spacer_counter{0};
    std::vector<std::shared_ptr<ColumnObj>> objs; //begin() ColumnObj->index = 1

    std::shared_ptr<ColumnObj> get_last(std::shared_ptr<ColumnObj> iterator);
    size_t count_elements_start_header(std::shared_ptr<ColumnObj> iterator);
    void insert(std::shared_ptr<ColumnObj>& last, std::shared_ptr<ColumnObj>& nu, size_t index);
};

}
#endif // DANCINGLINKS_H
