#ifndef DANCINGLINKS_H
#define DANCINGLINKS_H

#include <memory>
#include<unordered_map>
#include <vector>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>

namespace sudoku::dancinglinks {

class DancingLinks
{
    std::vector<char> letters;
    int spacer_counter{0};
    size_t header_size{0}; // no root
public:

    struct ColumnObj{
        int size {0};
        std::string name {""};
        int32_t index {0};
        int32_t top_spacer {0};
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
        std::shared_ptr<ColumnObj> last;
        for(size_t i = 0; i < size; ++i){
            std::shared_ptr<ColumnObj> nu = std::make_shared<ColumnObj>();
            if (i == 0){
                insert(root, nu, i);
                last = nu;
            } else if (i == size - 1){
                insert(last, nu, i);
                nu->right = root;
                root->left = nu;
            } else{
                insert(last, nu, i);
                last = nu;
            }
            objs.push_back(nu);
        }
        header_size = objs.size();
    }

    void insert(std::shared_ptr<ColumnObj>& last, std::shared_ptr<ColumnObj>& nu, size_t index){
        last->right = nu;
        nu->left = last;
        nu->name = letters.at(index);
        nu->index = index +1;
        nu->is_header = true;
    }

    std::shared_ptr<ColumnObj> get_last(std::shared_ptr<ColumnObj> obj){
        if(obj->down == nullptr)
            return obj;
        std::shared_ptr<ColumnObj>& iterator = obj;
        while(iterator){
            if(!iterator->down){
                return iterator;
            }
            iterator = iterator->down;
        }
    }

    std::shared_ptr<ColumnObj> get_last_spacer(){
        auto result = std::find_if(objs.rbegin(), objs.rend(),
                             [](std::shared_ptr<ColumnObj>& obj){
                                return obj->index < 0;
                            });
        return result == objs.rend() ? nullptr : *result;
    }

    std::shared_ptr<ColumnObj>& get_header(int32_t index){
        assert(index > 0);
        return objs.at(index - 1);
    }

    void make_row(std::vector<uint16_t>& data){
        assert(data.size() == header_size);
        int last_index{objs.back()->index};
        std::shared_ptr<ColumnObj> spacer = std::make_shared<ColumnObj>();
        spacer->up = get_last_spacer();
        spacer->top_spacer = spacer_counter--;
        spacer->index = ++last_index;
        objs.push_back(spacer);

        std::shared_ptr<ColumnObj> last_obj;
        for(size_t  i = 0; i < data.size(); ++i){
            if(data.at(i) == 1){
                std::shared_ptr<ColumnObj>& header = objs.at(i);
                auto last = get_last(header);
                std::shared_ptr<ColumnObj> new_obj = std::make_shared<ColumnObj>();
                new_obj->top = header;
                new_obj->up = last;
                new_obj->index = ++last_index;
                last->down = new_obj;
                objs.push_back(new_obj);
            }
        }
        spacer->down =  last_obj;
    }




};



}
#endif // DANCINGLINKS_H
