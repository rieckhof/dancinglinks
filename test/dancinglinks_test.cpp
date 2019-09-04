#include "dancinglinks.h"
#include <gtest/gtest.h>

using namespace sudoku::dancinglinks;

struct TestingStuff{
    TestingStuff* left;
    TestingStuff* right;
};

TEST(DancingLinks, ShouldInsertHeader){
    DancingLinks dl;
    std::shared_ptr<DancingLinks::ColumnObj> h1 {std::make_shared<DancingLinks::ColumnObj>()};
    h1->name = 1;
    std::shared_ptr<DancingLinks::ColumnObj> h2 {std::make_shared<DancingLinks::ColumnObj>()};
    dl.insert(h1,h2,2);

    ASSERT_EQ(h1->right, h2);
    ASSERT_EQ(h2->left, h1);
    ASSERT_EQ(h2->name,"C");
}

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
    ASSERT_EQ(dl.get_header(1)->down->index,6);
    ASSERT_EQ(dl.get_header(1)->down->down->index,9);
    ASSERT_EQ(dl.get_header(4)->down->index,7);
    ASSERT_EQ(dl.get_header(3)->down->index,10);
    ASSERT_EQ(dl.get_header(5)->top_spacer,-0);
    ASSERT_EQ(dl.get_header(5)->top,nullptr);
    ASSERT_EQ(dl.get_header(5)->down->index, 7);
    ASSERT_EQ(dl.get_header(8)->top_spacer,-1);

}

