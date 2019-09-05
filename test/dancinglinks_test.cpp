#include "dancinglinks.h"
#include <gtest/gtest.h>

using namespace sudoku::dancinglinks;

struct TestingStuff{
    TestingStuff* left;
    TestingStuff* right;
};

TEST(DancingLinks, ShouldCreateHeaderOf3Length){
    DancingLinks dl;
    const static size_t header_size (3);
    dl.make_header(header_size);
    ASSERT_TRUE(dl.root->name.empty());
    ASSERT_EQ(dl.root->right->name, "A");
    ASSERT_EQ(dl.root->right->right->name, "B");
    ASSERT_EQ(dl.root->right->right->right->name, "C");
    ASSERT_EQ(dl.root->right->right->right->right, dl.root);
    ASSERT_EQ(dl.root->right->right->right->right->right->name, dl.root->right->name);
    ASSERT_EQ(dl.root->right->right->right->right->right, dl.root->right);

    ASSERT_EQ(dl.root->left->name, "C");
    ASSERT_EQ(dl.root->left->left->name, "B");
    ASSERT_EQ(dl.root->left->left->left->name, "A");
    ASSERT_EQ(dl.root->left->left->left->left, dl.root);
    ASSERT_EQ(dl.root->left->left->left->left->left->name, dl.root->left->name);
    ASSERT_EQ(dl.root->left->left->left->left->left, dl.root->left);
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
    dl.print();

}


