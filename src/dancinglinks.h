#ifndef DANCINGLINKS_H
#define DANCINGLINKS_H

#include <memory>
#include<unordered_map>
#include <vector>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>
#include <map>

namespace sudoku::dancinglinks {

class DancingLinks
{
    std::vector<char> letters;
    int spacer_counter{0};
    size_t header_size{0}; // no root
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

    std::vector<std::shared_ptr<ColumnObj>> objs; //begin() ColumnObj->index = 1

    std::shared_ptr<ColumnObj> root {std::make_shared<ColumnObj>()};
    std::unordered_map<size_t,ColumnObj> index;

    DancingLinks();

    void make_header(size_t size);

    std::shared_ptr<ColumnObj> get_object(uint32_t index);

    void create_matrix(std::map<size_t, std::vector<uint16_t>>const& board);

    void make_row(std::vector<uint16_t>const& data);

    std::shared_ptr<ColumnObj> get_last_spacer();

    void print();

    void solve_algo_X(){

    }

private:

    std::shared_ptr<ColumnObj> get_last(std::shared_ptr<ColumnObj> iterator);

    size_t count_elements_start_header(std::shared_ptr<ColumnObj> iterator);

    void insert(std::shared_ptr<ColumnObj>& last, std::shared_ptr<ColumnObj>& nu, size_t index);

};

}
#endif // DANCINGLINKS_H
