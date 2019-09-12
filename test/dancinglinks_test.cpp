#include "dancinglinks.h"
#include <gtest/gtest.h>
#include "utils.h"

using namespace sudoku::dancinglinks;

struct TestingStuff{
    TestingStuff* left;
    TestingStuff* right;
};

TEST(DancingLinks, ShouldCreateHeaderOf3Length){
    DancingLinks dl;
    const static size_t header_size (3);
    dl.make_header(header_size);
    ASSERT_TRUE(dl.get_root()->name.empty());
    ASSERT_EQ(dl.get_root()->right->name, "A");
    ASSERT_EQ(dl.get_root()->right->right->name, "B");
    ASSERT_EQ(dl.get_root()->right->right->right->name, "C");
    ASSERT_EQ(dl.get_root()->right->right->right->right, dl.get_root());
    ASSERT_EQ(dl.get_root()->right->right->right->right->right->name, dl.get_root()->right->name);
    ASSERT_EQ(dl.get_root()->right->right->right->right->right, dl.get_root()->right);
    ASSERT_EQ(dl.get_root()->left->name, "C");
    ASSERT_EQ(dl.get_root()->left->left->name, "B");
    ASSERT_EQ(dl.get_root()->left->left->left->name, "A");
    ASSERT_EQ(dl.get_root()->left->left->left->left, dl.get_root());
    ASSERT_EQ(dl.get_root()->left->left->left->left->left->name, dl.get_root()->left->name);
    ASSERT_EQ(dl.get_root()->left->left->left->left->left, dl.get_root()->left);
}

TEST(DancingLinksShould, get_last_spacer_return_null){
    DancingLinks dl;
    dl.make_header(4);
    ASSERT_FALSE(dl.get_last_spacer());
}

TEST(DancingLinksShould, make_line_cerates_line){
    DancingLinks dl;
    dl.make_header(4);
    std::vector<uint16_t> row1 {1,0,0,1};
    std::vector<uint16_t> row2 {1,0,1,0};
    dl.make_row(row1);
    dl.make_row(row2);
    ASSERT_EQ(dl.get_object(1)->down->index,6);
    ASSERT_EQ(dl.get_object(1)->down->down->index,9);
    ASSERT_EQ(dl.get_object(4)->down->index,7);
    ASSERT_EQ(dl.get_object(3)->down->index,10);

    ASSERT_EQ(dl.get_object(5)->top_spacer,0);
    ASSERT_EQ(dl.get_object(5)->down->index, 7);
    ASSERT_EQ(dl.get_object(5)->up,nullptr);

    ASSERT_EQ(dl.get_object(8)->top_spacer,-1);
    ASSERT_EQ(dl.get_object(8)->down->index, 10);
    ASSERT_EQ(dl.get_object(8)->up->index, 6);
}


TEST(DancingLinks, createMatrix){
    DancingLinks dl;
    std::map<size_t, std::vector<uint16_t>> b;
    b.insert({0, {1, 1, 0, 0, 0}});
    b.insert({1, {1, 1, 1, 0, 0}});
    b.insert({3, {1, 1, 0, 1, 0}});
    b.insert({4, {0, 0, 1, 0, 1}});
    b.insert({5, {0, 0, 1, 1, 1}});
    dl.create_matrix(b);
    std::vector<std::vector<size_t>> all_expected_cols{{1,7,10,14},
                {2,8,11,15}, {3,12,18,21},
                {4,16,22}, {5,19,23}};
    check_all_directions(dl, all_expected_cols);
}

TEST(DancingLinks, createMatrixWith0Row){
    DancingLinks dl;
    std::map<size_t, std::vector<uint16_t>> b;
    b.insert({0, {1, 1, 0, 0, 0}});
    b.insert({1, {1, 1, 1, 0, 0}});
    b.insert({3, {1, 1, 0, 1, 0}});
    b.insert({4, {0, 0, 0, 0, 0}});
    b.insert({5, {0, 0, 1, 1, 1}});
    dl.create_matrix(b);
    std::vector<std::vector<size_t>> all_expected_cols{{1,7,10,14},
                {2,8,11,15}, {3,12,19},
                {4,16,20}, {5,21}};
    check_all_directions(dl, all_expected_cols);
}

TEST(DancingLinks, createMatrixWith2_0Row){
    DancingLinks dl;
    std::map<size_t, std::vector<uint16_t>> b;
    b.insert({0, {1, 1, 0, 0, 0}});
    b.insert({1, {1, 1, 1, 0, 0}});
    b.insert({3, {0, 0, 0, 0, 0}});
    b.insert({4, {0, 0, 0, 0, 0}});
    b.insert({5, {0, 0, 1, 1, 1}});
    dl.create_matrix(b);
    std::vector<std::vector<size_t>> all_expected_cols{{1,7,10},
                                                       {2,8,11}, {3,12,16},
                                                       {4,17}, {5,18}};
    check_all_directions(dl, all_expected_cols);
}

TEST(DancingLinks, SpacerTest){
    DancingLinks dl;
    std::map<size_t, std::vector<uint16_t>> b;
    b.insert({0, {1, 1, 0, 0, 0}});
    b.insert({1, {1, 1, 1, 0, 0}});
    b.insert({3, {1, 1, 0, 1, 0}});
    b.insert({4, {0, 0, 1, 0, 1}});
    b.insert({5, {0, 0, 1, 1, 1}});
    dl.create_matrix(b);
//    dl.print();

    auto last_spacer = dl.get_last_spacer();
    auto first_spacer = dl.get_object(6);
    auto second_spacer = dl.get_object(9);
    auto second_last_spacer = dl.get_object(20);

    ASSERT_EQ(first_spacer->index, 6);
    ASSERT_EQ(first_spacer->top_spacer, 0);
    ASSERT_EQ(first_spacer->down->index, 8);
    ASSERT_EQ(first_spacer->top, nullptr);

    ASSERT_EQ(second_spacer->index, 9);
    ASSERT_EQ(second_spacer->top_spacer, -1);
    ASSERT_EQ(second_spacer->up->index, 7);
    ASSERT_EQ(second_spacer->down->index, 12);

    ASSERT_EQ(second_last_spacer->index, 20);
    ASSERT_EQ(second_last_spacer->top_spacer, -4);
    ASSERT_EQ(second_last_spacer->up->index, 18);
    ASSERT_EQ(second_last_spacer->down->index, 23);

    ASSERT_EQ(last_spacer->index, 24);
    ASSERT_EQ(last_spacer->top_spacer, -5);
    ASSERT_EQ(last_spacer->up->index, 21);
}

TEST(CreateBoard, create_empty_initial_board2){
    size_t board_size(2);
    size_t should_lines(board_size * board_size *board_size);
    TheBoardComplex c(board_size);
    SodokuMap map;
    map.insert({"00",0});
    map.insert({"01",0});
    map.insert({"10",0});
    map.insert({"11",0});
    TheBoard b = c.create_initial_board(map);
    ASSERT_EQ(should_lines,b.size());

    TheBoard expected_board;
    expected_board.insert({0, {1 , 0 , 0 ,0, 1, 0,0, 0, 1, 0, 0, 0}});
    expected_board.insert({1, {1 , 0 , 0 ,0, 0, 1,0, 0, 0, 1, 0, 0}});
    expected_board.insert({2, {0 , 1 , 0 ,0, 1, 0,0, 0, 0, 0, 1, 0}});
    expected_board.insert({3, {0 , 1 , 0 ,0, 0, 1,0, 0, 0, 0, 0, 1}});
    expected_board.insert({4, {0 , 0 , 1 ,0, 0, 0,1, 0, 1, 0, 0, 0}});
    expected_board.insert({5, {0 , 0 , 1 ,0, 0, 0,0, 1, 0, 1, 0, 0}});
    expected_board.insert({6, {0 , 0 , 0 ,1, 0, 0,1, 0, 0, 0, 1, 0}});
    expected_board.insert({7, {0 , 0 , 0 ,1, 0, 0,0, 1, 0, 0, 0, 1}});

    ASSERT_EQ(expected_board, b);
}
TEST(CreateBoard, create_initial_board2){
    size_t board_size(2);
    size_t should_lines(board_size * board_size *board_size);
    TheBoardComplex c(board_size);
    SodokuMap map;
    map.insert({"00",1});
    map.insert({"01",2});
    map.insert({"10",2});
    map.insert({"11",0});
    TheBoard b = c.create_initial_board(map);
    ASSERT_EQ(should_lines,b.size());

    TheBoard expected_board;
    expected_board.insert({0, {1 , 0 , 0 , 0, 1, 0,0, 0, 1, 0, 0, 0}});
    expected_board.insert({1, {0 , 0 , 0 , 0, 0, 0,0, 0, 0, 0, 0, 0}});
    expected_board.insert({2, {0 , 0 , 0 , 0, 0, 0,0, 0, 0, 0, 0, 0}});
    expected_board.insert({3, {0 , 1 , 0 , 0, 0, 1,0, 0, 0, 0, 0, 1}});
    expected_board.insert({4, {0 , 0 , 0 , 0, 0, 0,0, 0, 0, 0, 0, 0}});
    expected_board.insert({5, {0 , 0 , 1 , 0, 0, 0,0, 1, 0, 1, 0, 0}});
    expected_board.insert({6, {0 , 0 , 0 , 1, 0, 0,1, 0, 0, 0, 1, 0}});
    expected_board.insert({7, {0 , 0 , 0 , 1, 0, 0,0, 1, 0, 0, 0, 1}});

    ASSERT_EQ(expected_board, b);
}

TEST(CreateBoard, create_empty_initial_board3){
    size_t board_size(3);
    size_t should_lines(board_size * board_size *board_size);
    TheBoardComplex c(board_size);
    SodokuMap map;
    map.insert({"00",0});
    map.insert({"01",0});
    map.insert({"02",0});
    map.insert({"10",0});
    map.insert({"11",0});
    map.insert({"12",0});
    map.insert({"20",0});
    map.insert({"21",0});
    map.insert({"22",0});
    TheBoard b = c.create_initial_board(map);
    ASSERT_EQ(should_lines,b.size());

    TheBoard expected_board;
    expected_board.insert({0, { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({1, { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({2, { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({3, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }});
    expected_board.insert({4, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }});
    expected_board.insert({5, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }});
    expected_board.insert({6, { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }});
    expected_board.insert({7, { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }});
    expected_board.insert({8, { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }});
    expected_board.insert({9, { 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({10, { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({11, { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({12, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }});
    expected_board.insert({13, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }});
    expected_board.insert({14, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }});
    expected_board.insert({15, { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }});
    expected_board.insert({16, { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }});
    expected_board.insert({17, { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }});
    expected_board.insert({18, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({19, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({20, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({21, { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }});
    expected_board.insert({22, { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }});
    expected_board.insert({23, { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0 }});
    expected_board.insert({24, { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }});
    expected_board.insert({25, { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }});
    expected_board.insert({26, { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 }});

    ASSERT_EQ(expected_board, b);
    DancingLinks dl;
    dl.create_matrix(b);
}


TEST(CreateBoard, create_initial_board3){
    size_t board_size(3);
    size_t should_lines(board_size * board_size *board_size);
    TheBoardComplex c(board_size);
    SodokuMap map;
    map.insert({"00",3});
    map.insert({"01",1});
    map.insert({"02",0});
    map.insert({"10",1});
    map.insert({"11",0});
    map.insert({"12",0});
    map.insert({"20",0});
    map.insert({"21",0});
    map.insert({"22",0});
    TheBoard b = c.create_initial_board(map);
    ASSERT_EQ(should_lines,b.size());

    TheBoard expected_board;
    expected_board.insert({0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({1, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({2, { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({3, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }});
    expected_board.insert({4, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({5, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({6, { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }});
    expected_board.insert({7, { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }});
    expected_board.insert({8, { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }});
    expected_board.insert({9, { 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({10, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({11, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({12, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }});
    expected_board.insert({13, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }});
    expected_board.insert({14, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }});
    expected_board.insert({15, { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }});
    expected_board.insert({16, { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }});
    expected_board.insert({17, { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }});
    expected_board.insert({18, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({19, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({20, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 }});
    expected_board.insert({21, { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }});
    expected_board.insert({22, { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }});
    expected_board.insert({23, { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0 }});
    expected_board.insert({24, { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }});
    expected_board.insert({25, { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }});
    expected_board.insert({26, { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 }});

    ASSERT_EQ(expected_board, b);
}

TEST(get_box_index_first_3_should, return_values_for_4){
    TheBoardComplex c(3);
    auto result = c.get_box_index(0,4,2);
    EXPECT_EQ(0,result);

    result = c.get_box_index(2,4,2);
    EXPECT_EQ(0,result);

    result = c.get_box_index(32,4,2);
    EXPECT_EQ(2,result);

    result = c.get_box_index(31,4,2);
    EXPECT_EQ(1,result);

    result = c.get_box_index(63,4,2);
    EXPECT_EQ(3,result);
}

TEST(get_box_index_first_3_should, return_values_for_9){
    TheBoardComplex c(3);
    auto result = c.get_box_index(0,9,3);
    EXPECT_EQ(0,result);

    result = c.get_box_index(26,9,3);
    EXPECT_EQ(0,result);

    result = c.get_box_index(27,9,3);
    EXPECT_EQ(1,result);

    result = c.get_box_index(53,9,3);
    EXPECT_EQ(1,result);

    result = c.get_box_index(54,9,3);
    EXPECT_EQ(2,result);

    result = c.get_box_index(80,9,3);
    EXPECT_EQ(2,result);

    result = c.get_box_index(81,9,3);
    EXPECT_EQ(0,result);

    result = c.get_box_index(107,9,3);
    EXPECT_EQ(0,result);

    result = c.get_box_index(108,9,3);
    EXPECT_EQ(1,result);

    result = c.get_box_index(134,9,3);
    EXPECT_EQ(1,result);

    result = c.get_box_index(135,9,3);
    EXPECT_EQ(2,result);

    result = c.get_box_index(161,9,3);
    EXPECT_EQ(2,result);

    result = c.get_box_index(162,9,3);
    EXPECT_EQ(0,result);

    result = c.get_box_index(242,9,3);
    EXPECT_EQ(2,result);

    result = c.get_box_index(243,9,3);
    EXPECT_EQ(3,result);

    result = c.get_box_index(728,9,3);
    EXPECT_EQ(8,result);
}

