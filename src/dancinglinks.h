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

    DancingLinks() : letters (35){
        int begin (65);
        std::generate_n(letters.begin(), 35, [&begin](){
            return static_cast<char>(begin++);
        });
    }

    void make_header(size_t size){
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

    std::shared_ptr<ColumnObj> get_object(uint32_t index){
        assert(index > 0);
        return objs.at(index - 1);
    }

    void create_matrix(std::map<size_t, std::vector<uint16_t>>const& board){
        make_header(board.at(0).size());
        for(auto& [index, data] : board){
            make_row(data);
        }
        std::for_each(objs.begin(), objs.begin() + header_size,
                      [this](std::shared_ptr<ColumnObj>& header){
                            header->size = count_elements_start_header(header);
                            auto last = get_last(header);
                            header->up = last;
                            last->down = header;
                      });
    }


    void make_row(std::vector<uint16_t>const& data){
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

    std::shared_ptr<ColumnObj> get_last_spacer(){
        auto result = std::find_if(objs.rbegin(), objs.rend(),
                             [](std::shared_ptr<ColumnObj>& obj){
                                return obj->top_spacer <= 0;
                            });
        return result == objs.rend() ? nullptr : *result;
    }

    void print(){
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

private:

    std::shared_ptr<ColumnObj> get_last(std::shared_ptr<ColumnObj> iterator){
        while(iterator){
            if(!iterator->down){
                return iterator;
            }
            iterator = iterator->down;
        }
    }
    size_t count_elements_start_header(std::shared_ptr<ColumnObj> iterator){
        size_t result(0);
        while(iterator){
            if(!iterator->down){
                return result;
            }
            ++result;
            iterator = iterator->down;
        }
    }

    void insert(std::shared_ptr<ColumnObj>& last, std::shared_ptr<ColumnObj>& nu, size_t index){
        last->right = nu;
        nu->left = last;
        nu->name = letters.at(index);
        nu->index = index +1;
        nu->is_header = true;
    }

};

}
#endif // DANCINGLINKS_H
