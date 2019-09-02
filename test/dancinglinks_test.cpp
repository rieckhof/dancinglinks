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
