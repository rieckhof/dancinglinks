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
    std::vector<std::vector<size_t>> all_expected_values{{1,7,10,14},
                {2,8,11,15}, {3,12,18,21},
                {4,16,22}, {5,19,23}};
    check_all_directions(dl, all_expected_values);
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
