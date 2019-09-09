#include "dancinglinks.h"
#include <algorithm>

namespace sudoku::dancinglinks {

std::shared_ptr<DancingLinks::ColumnObj>const& DancingLinks::get_root() {
    return root;
};

std::shared_ptr<DancingLinks::ColumnObj> DancingLinks::get_item_with_least_options(){
    std::shared_ptr<ColumnObj> result;
    std::for_each(objs.begin(), objs.begin() + get_header_size(),
                  [&result](std::shared_ptr<ColumnObj>& header){
                            if(!result or result->size > header->size){
                                result = header;
                            }
                    });
    return result;
}

int32_t DancingLinks::get_header_size() const{
    return static_cast<int32_t>(header_size);
}

DancingLinks::DancingLinks() : letters(number_of_leters){
    int begin (ascii_code_for_A);
    std::generate_n(letters.begin(), number_of_leters, [&begin](){
        return static_cast<char>(begin++);
    });
}

void DancingLinks::make_header(size_t size){
    std::shared_ptr<ColumnObj> last = root;
    for(size_t i = 0; i < size; ++i){
        std::shared_ptr<ColumnObj> nu = std::make_shared<ColumnObj>();
        insert(last, nu, i);
        if (i == size - 1){
            nu->right = root;
            root->left = nu;
        }else{
            last = nu;
        }
        objs.push_back(nu);
    }
    header_size = objs.size();
}

std::shared_ptr<DancingLinks::ColumnObj> DancingLinks::get_object(size_t index) const{
    assert(index > 0);
    return objs.at(index - 1);
}

void DancingLinks::create_matrix(std::map<size_t, std::vector<uint16_t>>const& board){
    make_header(board.at(0).size());
    for(auto& [index, data] : board){
        make_row(data);
    }
    std::for_each(objs.begin(), objs.begin() + get_header_size(),
                  [this](std::shared_ptr<ColumnObj>& header){
                        header->size = count_elements_start_header(header);
                        auto last = get_last(header);
                        header->up = last;
                        last->down = header;
                  });
}


void DancingLinks::make_row(std::vector<uint16_t>const& data){
    assert(data.size() == header_size);

    size_t last_index{objs.back()->index};
    std::shared_ptr<ColumnObj> spacer = std::make_shared<ColumnObj>();
    auto last_spacer = get_last_spacer();
    spacer->up = last_spacer ? get_object(last_spacer->index + 1) : nullptr;
    spacer->top_spacer = spacer_counter--;
    spacer->index = ++last_index;
    objs.push_back(spacer);

    std::shared_ptr<ColumnObj> last_obj;
    for(size_t  i = 0; i < data.size(); ++i){
        if(data.at(i) == 1){
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
    spacer->down =  last_obj;
}

std::shared_ptr<DancingLinks::ColumnObj> DancingLinks::get_last_spacer(){
    auto result = std::find_if(objs.rbegin(), objs.rend(),
                         [](std::shared_ptr<ColumnObj>& obj){
                            return obj->top_spacer <= 0;
                        });
    return result == objs.rend() ? nullptr : *result;
}

void DancingLinks::print() const{
    std::shared_ptr<ColumnObj> iterator = root->right;
    while(iterator != root){
        std::cout << "Header " << iterator->index << " " << iterator->size << std::endl;
        std::cout << "|"<< std::endl;
        std::cout << "v"<< std::endl;
        auto iterator_down = iterator->down;
        while(iterator_down and iterator_down != iterator){
            std::cout << "Element " << iterator_down->index << std::endl;
            std::cout << "|"<< std::endl;
            std::cout << "v"<< std::endl;
            iterator_down = iterator_down->down;
        }
        iterator = iterator->right;
    }
}

std::shared_ptr<DancingLinks::ColumnObj> DancingLinks::get_last(std::shared_ptr<ColumnObj> iterator){
    while(iterator){
        if(!iterator->down){
            return iterator;
        }
        iterator = iterator->down;
    }
}

size_t DancingLinks::count_elements_start_header(std::shared_ptr<ColumnObj> iterator){
    size_t result(0);
    while(iterator){
        if(!iterator->down){
            return result;
        }
        ++result;
        iterator = iterator->down;
    }
}

void DancingLinks::insert(std::shared_ptr<ColumnObj>& last, std::shared_ptr<ColumnObj>& nu, size_t index){
    last->right = nu;
    nu->left = last;
    nu->name = letters.at(index);
    nu->index = index +1;
    nu->is_header = true;
}
}
