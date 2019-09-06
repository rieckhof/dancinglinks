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
#include <limits>
#include <unordered_set>

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
    std::unordered_set<std::shared_ptr<ColumnObj>> backtracking_helper;
    std::vector<std::shared_ptr<ColumnObj>> solution;

    std::shared_ptr<ColumnObj> root {std::make_shared<ColumnObj>()};
    std::unordered_map<size_t,ColumnObj> index;


    DancingLinks();

    void make_header(size_t size);

    std::shared_ptr<ColumnObj> get_object(size_t index);

    void create_matrix(std::map<size_t, std::vector<uint16_t>>const& board);

    void make_row(std::vector<uint16_t>const& data);

    std::shared_ptr<ColumnObj> get_last_spacer();

    void print();

    std::shared_ptr<ColumnObj> get_item_with_least_elements(){
        std::shared_ptr<ColumnObj> result;
        std::for_each(objs.begin(), objs.begin() + header_size,
                      [&result](std::shared_ptr<ColumnObj>& header){
                                if(!result or result->size > header->size){
                                    result = header;
                                }
                        });
        return result;
    }

    void hide(std::shared_ptr<ColumnObj>& p){
        auto q = get_object(p->index + 1);
        while(p != q){
            auto& x = q->top;
            auto& u = q->up;
            auto& d = q->down;
            if(x == nullptr){
                q = u;
            }else{
                d = u->down;
                u = d->up;
                --x->size;
                q = get_object(q->index +1);
            }
        }
    }

    void cover(std::shared_ptr<ColumnObj>& header){
        auto p = header->down;
        while(p != header){
            hide(p);
            p = p->down;
        }
        auto& l = header->left;
        auto& r = header->right;
        r = l->right;
        l = r->left;
    }

    void unhide(std::shared_ptr<ColumnObj>& p){
        auto q = get_object(p->index -1);
        while(q  != p){
            auto& x = q->top;
            auto& u = q->up;
            auto& d = q->down;
            if(x == nullptr){
                q = d;
            }else{
                q = u->down;
                q = d->up;
                ++x->size;
                q = get_object(q->index -1);
            }
        }
    }

    void uncover(std::shared_ptr<ColumnObj>& header){
        auto& l = header->left;
        auto& r = header->right;
        header = l->right;
        header = r->left;
        auto p = header->up;
        while(p != header){
            unhide(p);
            p = p->up;
        }
    }

    void create_solution(std::unordered_set<std::shared_ptr<ColumnObj>>& bk_helper){
        for(auto& x : bk_helper){
            solution.push_back(x->top);
            auto next = get_object(x->index +1);
            while(x != next){
                auto sol = next->top;
                if(sol == nullptr){
                    next = next->up;
                }else{
                    solution.push_back(sol);
                    next = get_object(x->index +1);
                }
            }
        }
    }

    void solve_algo_X(int& level){
        if(root->right == root){
            create_solution(backtracking_helper);
            if(level == 0){
                return;
            }else {
                --level;
                return;
            }
        }
        auto item_i= get_item_with_least_elements();
        cover(item_i);
        auto x =  item_i->down;
        if(x == item_i){
            uncover(item_i);
        }else{
            while(x == item_i){
                //Start X5
                backtracking_helper.insert(x);
                auto p = get_object(x->index + 1);
                while(p != x){
                    auto j = p->top;
                    if(j == nullptr){
                        p = p->up;
                    }else{
                        cover(j);
                        p = get_object(p->index +1);
                    }
                }
                //End X5
                solve_algo_X(++level); //goto X2
                //Start X6
                backtracking_helper.erase(x);
                p = get_object(x->index - 1);
                while(p != x){
                    auto j = p->top;
                    if(j == nullptr){
                        p = p->down;
                    }else{
                        uncover(j);
                        p = get_object(p->index -1);
                    }
                }
                item_i = x->top;
                x = x->down;
                //End X6
            }
            uncover(item_i);
        }
    }

private:

    std::shared_ptr<ColumnObj> get_last(std::shared_ptr<ColumnObj> iterator);

    size_t count_elements_start_header(std::shared_ptr<ColumnObj> iterator);

    void insert(std::shared_ptr<ColumnObj>& last, std::shared_ptr<ColumnObj>& nu, size_t index);

};

}
#endif // DANCINGLINKS_H
