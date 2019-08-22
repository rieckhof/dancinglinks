#include "dancinglinks.h"
#include <gtest/gtest.h>

using namespace sudoku::dancinglinks;

struct TestingStuff{
    TestingStuff* left;
    TestingStuff* right;
};

TEST(DancingLinks,ShouldInsertHeader){
    DancingLinks dl;
    std::shared_ptr<DancingLinks::Header> h1 {std::make_shared<DancingLinks::Header>()};
    h1->name = 1;
    std::shared_ptr<DancingLinks::Header> h2 {std::make_shared<DancingLinks::Header>()};
    dl.insert(h1,h2,2);

    ASSERT_EQ(h1->right, h2);
    ASSERT_EQ(h2->left, h1);
    ASSERT_EQ(h2->name,2);
}

TEST(DancingLinks, ShouldCreateHeaderOf3Length){
    DancingLinks dl;
    const static size_t header_size (3);
    dl.make_header(header_size);
    ASSERT_EQ(dl.root->name, -1);
    ASSERT_EQ(dl.root->right->name, 0);
    ASSERT_EQ(dl.root->right->right->name, 1);
    ASSERT_EQ(dl.root->right->right->right->name, 2);
    ASSERT_EQ(dl.root->right->right->right->right->name, -1);
    ASSERT_EQ(dl.root->right->right->right->right->right->name, dl.root->right->name);
    ASSERT_EQ(dl.root->right->right->right->right->right, dl.root->right);

    ASSERT_EQ(dl.root->left->name, 2);
    ASSERT_EQ(dl.root->left->left->name, 1);
    ASSERT_EQ(dl.root->left->left->left->name, 0);
    ASSERT_EQ(dl.root->left->left->left->left->name, -1);
    ASSERT_EQ(dl.root->left->left->left->left->left->name, dl.root->left->name);
    ASSERT_EQ(dl.root->left->left->left->left->left, dl.root->left);

}
