#ifndef DANCINGLINKS_H
#define DANCINGLINKS_H

#include <memory>
#include<unordered_map>
#include <vector>
#include <algorithm>

class DancingLinks
{
    struct ColumnObj {
        std::shared_ptr<ColumnObj> right;
        std::shared_ptr<ColumnObj> left;
        std::shared_ptr<ColumnObj> down;
        std::shared_ptr<ColumnObj> up;
        std::shared_ptr<ColumnObj> col_head;
        int value;
    };
    struct Header : ColumnObj{
        int size;
        size_t name;
    };

    std::shared_ptr<Header> root;
    std::unordered_map<size_t,ColumnObj> index;

public:
    DancingLinks();
    void insert(std::shared_ptr<Header>& last, std::shared_ptr<Header>& nu, size_t name){
        last->right = nu;
        nu->left = last;
        nu->name = name;
        last = nu;
    }

    void make_header(size_t size){
        std::shared_ptr<Header> last;
        for(size_t i = 0; i < size; ++i){
            std::shared_ptr<Header> nu = std::make_shared<Header>();
            if (i == 0){
                insert(root, nu, i);
            } else if (i == size - 1){
                insert(last, nu, i);
                nu->right = root;
                root->left = nu;
            } else{
                insert(last, nu, i);
            }
        }
    }

    ColumnObj make_row(size_t row_index, std::vector<uint16_t> data){
        std::vector<size_t> index_ones;
        long number_of_ones = std::count_if(data.begin(),data.end(),[](uint16_t i){
           return i == 1;
        });
        index_ones.reserve(number_of_ones);
        for(size_t  i = 0; i < data.size(); ++i){
            if(data.at(i) == 1){
                index_ones.push_back(i);
            }
        }
        auto last_index = index_ones.back();
        std::shared_ptr<ColumnObj> last_element;
        for(size_t const& i : index_ones){
            std::shared_ptr<Header> head =
                    std::dynamic_pointer_cast<Header>(root->right);
            while(head){
                if(head->name == i){
                    std::shared_ptr<ColumnObj> col_new = std::make_shared<ColumnObj>();
                    if(last_element){
                        last_element->down = col_new;
                        col_new->up = last_element;

                        last_element->right = col_new;
                        col_new->left = last_element;
                        last_element = col_new;

                    }else{
                        last_element = col_new;
                        head->down = col_new;
                        head->col_head = col_new;
                        col_new->up = head;
                    }
                }
                head = std::dynamic_pointer_cast<Header>(head->right);
            }
        }
    }


};

#endif // DANCINGLINKS_H
