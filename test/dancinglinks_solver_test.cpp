#include "dancinglinks_solver.h"
#include <gtest/gtest.h>
#include <numeric>
#include "utils.h"

using namespace sudoku::dancinglinks;

using TheBoard = std::map<size_t, std::vector<uint16_t>>;
struct CoverTests : ::testing::Test {
  TheBoard b;
  std::vector<uint16_t> solution{{1, 1, 0, 1, 0}};
  std::vector<uint16_t> solution1{{0, 0, 1, 0, 1}};

  void SetUp() override {
    b.insert({0, {1, 1, 0, 0, 0}});
    b.insert({1, {1, 1, 1, 0, 0}});
    b.insert({2, solution});
    b.insert({3, solution1});
    b.insert({4, {0, 0, 1, 1, 1}});
  }
  std::vector<std::vector<size_t>> all_expected_values{{1, 7, 10, 14},
                                                       {2, 8, 11, 15},
                                                       {3, 12, 18, 21},
                                                       {4, 16, 22},
                                                       {5, 19, 23}};
};

TEST_F(CoverTests, CoverHeader1) {
  DancingLinksSolver dls(b);
  auto header1 = dls.dl.get_root()->right;
  ASSERT_EQ(header1->index, 1);
  dls.cover(header1);
  // matrix after cover
  // b.insert({4, {0, 1, 0, 1}});
  // b.insert({5, {0, 1, 1, 1}});
  auto header2 = dls.dl.get_root()->right;
  ASSERT_EQ(header2->index, 2);
  ASSERT_EQ(header2->left->index, dls.dl.get_root()->index);
  ASSERT_EQ(header2->down->index, 2);
  ASSERT_EQ(header2->up->index, 2);
  auto header3 = dls.dl.get_root()->right->right;
  ASSERT_EQ(header3->index, 3);
  ASSERT_EQ(header3->left->index, header2->index);
  ASSERT_EQ(header3->down->index, 18);
  ASSERT_EQ(header3->up->index, 21);
  ASSERT_EQ(header3->up->up->index, 18);
  ASSERT_EQ(header3->down->down->index, 21);
  auto header4 = dls.dl.get_root()->left->left;
  ASSERT_EQ(header4->index, 4);
  ASSERT_EQ(header4->down->index, 22);
  ASSERT_EQ(header4->down->down->index, 4);
  auto header5 = dls.dl.get_root()->left;
  ASSERT_EQ(header5->index, 5);
  ASSERT_EQ(header5->right, dls.dl.get_root());
  ASSERT_EQ(header5->down->index, 19);
  ASSERT_EQ(header5->down->down->index, 23);
}

TEST_F(CoverTests, CoverUncoverHeader1) {
  DancingLinksSolver dls(b);
  auto header1 = dls.dl.get_root()->right;
  ASSERT_EQ(header1->index, 1);

  dls.cover(header1);
  auto header2 = dls.dl.get_root()->right;
  ASSERT_EQ(header2->index, 2);
  dls.uncover(header1);

  check_all_directions(dls.dl, all_expected_values);
}

TEST_F(CoverTests, CoverHeader1And2) {
  DancingLinksSolver dls(b);
  auto header1 = dls.dl.get_root()->right;
  auto header2 = dls.dl.get_root()->right->right;
  ASSERT_EQ(header1->index, 1);
  dls.cover(header1);
  dls.cover(header2);
  // matrix after cover
  // b.insert({4, {1, 0, 1}});
  // b.insert({5, {1, 1, 1}});
  auto header3 = dls.dl.get_root()->right;
  ASSERT_EQ(header3->index, 3);
  ASSERT_EQ(header3->down->index, 18);
  ASSERT_EQ(header3->up->index, 21);
  ASSERT_EQ(header3->up->up->index, 18);
  ASSERT_EQ(header3->down->down->index, 21);
  auto header4 = dls.dl.get_root()->right->right;
  ASSERT_EQ(header4->index, 4);
  ASSERT_EQ(header4->down->index, 22);
  ASSERT_EQ(header4->down->down->index, 4);
  auto header5 = dls.dl.get_root()->right->right->right;
  ASSERT_EQ(header5->index, 5);
  ASSERT_EQ(header5->down->index, 19);
  ASSERT_EQ(header5->down->down->index, 23);
}

TEST_F(CoverTests, CoverHeader4) {
  DancingLinksSolver dls(b);
  auto& header4 = dls.dl.get_root()->left->left;
  ASSERT_EQ(header4->index, 4);
  dls.cover(header4);
  // matrix after cover
  //    b.insert({0, {1, 1, 0, 0}});
  //    b.insert({1, {1, 1, 1, 0}});
  //    b.insert({4, {0, 0, 1, 1}});
  auto& header1 = dls.dl.get_root()->right;
  auto& header2 = dls.dl.get_root()->right->right;
  auto& header3 = dls.dl.get_root()->right->right->right;
  auto& header5 = dls.dl.get_root()->right->right->right->right;

  ASSERT_EQ(header1->index, 1);
  ASSERT_EQ(header1->size, 2);
  ASSERT_EQ(header2->index, 2);
  ASSERT_EQ(header2->size, 2);
  ASSERT_EQ(header3->index, 3);
  ASSERT_EQ(header3->size, 2);
  ASSERT_EQ(header5->index, 5);
  ASSERT_EQ(header5->size, 1);

  ASSERT_EQ(header1->down->index, 7);
  ASSERT_EQ(header1->down->down->index, 10);
  ASSERT_EQ(header1->down->down->down->index, 1);

  ASSERT_EQ(header2->down->index, 8);
  ASSERT_EQ(header2->down->down->index, 11);
  ASSERT_EQ(header2->down->down->down->index, 2);

  ASSERT_EQ(header3->down->index, 12);
  ASSERT_EQ(header3->down->down->index, 18);
  ASSERT_EQ(header3->down->down->down->index, 3);

  ASSERT_EQ(header5->down->index, 19);
  ASSERT_EQ(header5->down->down->index, 5);
}

TEST_F(CoverTests, CoverUncoverHeader4) {
  DancingLinksSolver dls(b);
  auto header4 = dls.dl.get_root()->left->left;
  ASSERT_EQ(header4->index, 4);
  dls.cover(header4);
  dls.uncover(header4);

  check_all_directions(dls.dl, all_expected_values);
}

void evaluate_solution(std::vector<std::vector<size_t>> const& solutions,
                       std::map<size_t, std::vector<uint16_t>>& b) {
  for (auto& solution_indexes : solutions) {
    std::vector<uint16_t> complete_solution;
    std::vector<uint16_t> solution_part;
    for (size_t const& index4board : solution_indexes) {
      solution_part = b.at(index4board);
      complete_solution.insert(complete_solution.begin(), solution_part.begin(),
                               solution_part.end());
      //            std::cout << index4board << ": ";
      //            std::for_each(temp2.begin(), temp2.end(), [](auto
      //            i){std::cout << i << " ";}); std::cout << "\n";
    }
    //        std::cout << "\n";
    int total =
        std::accumulate(complete_solution.begin(), complete_solution.end(), 0,
                        std::plus<uint16_t>());
    EXPECT_EQ(total, solution_part.size());
  }
}

TEST_F(CoverTests, SolveSimpleProblem) {
  DancingLinksSolver dls(b);
  int level{0};
  dls.solve_algo_X(level);
  std::vector<std::vector<size_t>> results = dls.get_index_board();
  evaluate_solution(results, b);
}

TEST(CoverTests2, SolveSimpleProblem2) {
  TheBoard b;
  b.insert({0, {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}});
  b.insert({1, {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0}});
  b.insert({2, {0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0}});
  b.insert({3, {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}});
  b.insert({4, {0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0}});
  b.insert({5, {0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0}});
  b.insert({6, {0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0}});
  b.insert({7, {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1}});

  DancingLinksSolver dls(b);
  int level{0};
  dls.solve_algo_X(level);
  std::vector<std::vector<size_t>> results = dls.get_index_board();
  evaluate_solution(results, b);
}

TEST(CoverTests3, SolveSimpleProblem3) {
  TheBoard b;
  b.insert({0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}});
  b.insert({1, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}});
  b.insert({2, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}});
  b.insert({3, {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}});
  b.insert({4, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}});
  b.insert({5, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}});
  b.insert({6, {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}});
  b.insert({7, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
  b.insert({8, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}});
  b.insert({9, {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}});
  b.insert({10, {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}});
  b.insert({11, {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}});
  b.insert({12, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}});
  b.insert({13, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}});
  b.insert({14, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}});
  b.insert({15, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}});
  b.insert({16, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
  b.insert({17, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}});
  b.insert({18, {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}});
  b.insert({19, {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}});
  b.insert({20, {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0}});
  b.insert({21, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}});
  b.insert({22, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}});
  b.insert({23, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0}});
  b.insert({24, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}});
  b.insert({25, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}});
  b.insert({26, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1}});

  DancingLinksSolver dls(b);
  int level{0};
  dls.solve_algo_X(level);
  std::vector<std::vector<size_t>> results = dls.get_index_board();
  evaluate_solution(results, b);
}

TEST(Solver, ShouldSolveSimpleProbleSize2) {
  SodokuMap map;
  map.insert({"00", 1});
  map.insert({"01", 2});
  map.insert({"10", 2});
  map.insert({"11", 0});
  SudokuAdapter c(2);
  TheBoard b = c.create_initial_board(map);
  DancingLinksSolver dls(b);
  int level(0);
  dls.solve_algo_X(level);
  std::vector<std::vector<size_t>> results = dls.get_index_board();
  evaluate_solution(results, b);
}
TEST(Solver, ShouldSolveSimpleProbleSize3) {
  SodokuMap map;
  map.insert({"00", 3});
  map.insert({"01", 1});
  map.insert({"02", 0});
  map.insert({"10", 1});
  map.insert({"11", 0});
  map.insert({"12", 0});
  map.insert({"20", 0});
  map.insert({"21", 0});
  map.insert({"22", 0});
  SudokuAdapter c(3);
  TheBoard b = c.create_initial_board(map);
  DancingLinksSolver dls(b);
  int level(0);
  dls.solve_algo_X(level);
  std::vector<std::vector<size_t>> results = dls.get_index_board();
  evaluate_solution(results, b);
}

TEST(Solver, ShouldSolveSimpleProbleSize3WithSolution) {
  SodokuMap map;
  map.insert({"00", 3});
  map.insert({"01", 1});
  map.insert({"02", 0});
  map.insert({"10", 1});
  map.insert({"11", 0});
  map.insert({"12", 0});
  map.insert({"20", 0});
  map.insert({"21", 0});
  map.insert({"22", 0});

  std::vector<std::pair<std::string, u_int16_t>> expected_vals{
      {"00", 3}, {"01", 1}, {"02", 2}, {"10", 1}, {"11", 2},
      {"12", 3}, {"20", 2}, {"21", 3}, {"22", 1}};

  SudokuAdapter c(3);
  TheBoard b = c.create_initial_board(map);
  DancingLinksSolver dls(b);
  int level(0);
  dls.solve_algo_X(level);
  std::vector<SodokuMap> sol(dls.create_sudoku_solved(c));
  ASSERT_EQ(sol.size(), 1);
  for (SodokuMap m : sol) {
    //        for(auto& [index,val] : m){
    //            std::cout << index << " " << val << "\n";
    //        }
    for (auto& val : expected_vals) {
      ASSERT_EQ(val.second, m.at(val.first));
    }
  }
}
