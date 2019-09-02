#ifndef DANCINGLINKS_H
#define DANCINGLINKS_H

#include <memory>
#include<unordered_map>
#include <vector>
#include <algorithm>
#include <vector>
#include <iostream>

namespace sudoku::dancinglinks {

class DancingLinks
{
    std::vector<char> letters;
public:

    struct ColumnObj{
        int size {0};
        std::string name {""};
        int index {0};
        std::shared_ptr<ColumnObj> right;
        std::shared_ptr<ColumnObj> left;
        std::shared_ptr<ColumnObj> down;
        std::shared_ptr<ColumnObj> up;
        std::shared_ptr<ColumnObj> top;
    };
    std::vector<std::shared_ptr<ColumnObj>> objs;

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
    }

    void insert(std::shared_ptr<ColumnObj>& last, std::shared_ptr<ColumnObj>& nu, size_t index){
        last->right = nu;
        nu->left = last;
        nu->name = letters.at(index);
        nu->index = index;
    }

    ColumnObj make_row(size_t row_index, std::vector<uint16_t> data){
        std::shared_ptr<ColumnObj> first_spacer = std::make_shared<ColumnObj>();
        first_spacer->index = 0;
        for(size_t  i = 0; i < data.size(); ++i){
            if(data.at(i) == 1){

            }
        }
//        auto last_index = index_ones.back();
//        std::shared_ptr<ColumnObj> last_element;
//        for(size_t const& i : index_ones){
//            std::shared_ptr<Header> head = root;
//            while(head){
//                if(head->name == i){
//                    std::shared_ptr<ColumnObj> col_new = std::make_shared<ColumnObj>();
//                    if(last_element){
//                        last_element->down = col_new;
//                        col_new->up = last_element;

//                        last_element->right = col_new;
//                        col_new->left = last_element;
//                        last_element = col_new;

//                    }else{
//                        last_element = col_new;
//                        head->down = col_new;
//                        head->col_head = col_new;
//                        col_new->up = head;
//                    }
//                }
//                head = std::dynamic_pointer_cast<Header>(head->right);
//            }
//        }
    }




};



}
#endif // DANCINGLINKS_H
