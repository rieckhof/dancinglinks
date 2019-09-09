#ifndef DACINGLINK_SOLVER_H
#define DACINGLINK_SOLVER_H

#include "dancinglinks.h"
#include <unordered_set>
#include <algorithm>
#include <memory>

namespace sudoku::dancinglinks {

class DancingLinksSolver
{
    using ColumnObj = DancingLinks::ColumnObj;
    std::unordered_set<std::shared_ptr<ColumnObj>> backtracking_helper;
    std::vector<std::shared_ptr<ColumnObj>> solution;

public:
    DancingLinks dl;
    DancingLinksSolver(std::map<size_t, std::vector<uint16_t>>const& board){
        dl.create_matrix(board);
    }

    void hide(std::shared_ptr<ColumnObj>& p){
        auto q = dl.get_object(p->index + 1);
        while(p != q){
            auto x = q->top;
            auto u = q->up;
            auto d = q->down;
            if(x == nullptr){
                q = u;
            }else{
                u->down = d;
                d->up = u;
                --x->size;
                q = dl.get_object(q->index +1);
            }
        }
    }

    void cover(std::shared_ptr<ColumnObj>const& header){
        auto p = header->down;
        while(p != header){
            hide(p);
            p = p->down;
        }
        auto& l = header->left;
        auto& r = header->right;
        l->right = r;
        r->left = l;
    }

    void unhide(std::shared_ptr<ColumnObj>& p){
        auto q = dl.get_object(p->index -1);
        while(q  != p){
            auto x = q->top;
            auto u = q->up;
            auto d = q->down;
            if(x == nullptr){
                q = d;
            }else{
                u->down = q;
                d->up = q;
                ++x->size;
                q = dl.get_object(q->index -1);
            }
        }
    }

    void uncover(std::shared_ptr<ColumnObj>& header){
        auto& l = header->left;
        auto& r = header->right;
        l->right = header;
        r->left = header;
        auto p = header->up;
        while(p != header){
            unhide(p);
            p = p->up;
        }
    }

    void create_solution(std::unordered_set<std::shared_ptr<ColumnObj>>& bk_helper){
        for(auto& x : bk_helper){
            solution.push_back(x->top);
            auto next = dl.get_object(x->index +1);
            while(x != next){
                auto sol = next->top;
                if(sol == nullptr){
                    next = next->up;
                }else{
                    solution.push_back(sol);
                    next = dl.get_object(x->index +1);
                }
            }
        }
    }

    void solve_algo_X(int& level){
        if(dl.get_root()->right == dl.get_root()){
            create_solution(backtracking_helper);
            if(level == 0){
                return;
            }else {
                --level;
                return;
            }
        }
        auto item_i= dl.get_item_with_least_options();
        cover(item_i);
        auto x =  item_i->down;
        while(x != item_i){
            //Start X5
            backtracking_helper.insert(x);
            auto p = dl.get_object(x->index + 1);
            while(p != x){
                auto j = p->top;
                if(j == nullptr){
                    p = p->up;
                }else{
                    cover(j);
                    p = dl.get_object(p->index +1);
                }
            }
            //End X5
            solve_algo_X(++level); //goto X2
            //Start X6
            backtracking_helper.erase(x);
            p = dl.get_object(x->index - 1);
            while(p != x){
                auto j = p->top;
                if(j == nullptr){
                    p = p->down;
                }else{
                    uncover(j);
                    p = dl.get_object(p->index -1);
                }
            }
            item_i = x->top;
            x = x->down;
            //End X6
        }
        uncover(item_i);
    }
};

}

#endif // DACINGLINK_SOLVER_H
