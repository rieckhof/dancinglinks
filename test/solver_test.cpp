#include "board.h"
#include "serializer.h"
#include "solver.h"
#include "utils.h"
#include <gtest/gtest.h>

TEST(Utils, GetRangeFrom0to2) {
  std::vector<int> result = sudoku::utils::get_range(0, 2);
  ASSERT_EQ(3, result.size());
  ASSERT_EQ(0, result.at(0));
  ASSERT_EQ(1, result.at(1));
  ASSERT_EQ(2, result.at(2));
}

TEST(SolverShould, solveEasySUDOKU) {
  std::string easy_input{"{\"0\":[[],[],[6],[4],[],[9],[5],[],[]],"
                         "\"1\":[[],[],[3],[],[2],[],[1],[],[]],"
                         "\"2\":[[9],[1],[],[],[7],[],[],[2],[8]],"
                         "\"3\":[[6],[],[],[7],[],[3],[],[],[1]],"
                         "\"4\":[[],[4],[7],[],[8],[],[3],[5],[]],"
                         "\"5\":[[5],[],[],[2],[],[1],[],[],[9]],"
                         "\"6\":[[4],[2],[],[],[9],[],[],[3],[6]],"
                         "\"7\":[[],[],[1],[],[3],[],[9],[],[]],"
                         "\"8\":[[],[],[9],[8],[],[4],[2],[],[]]}"};

  sudoku::datastruct::Board b{::sudoku::serializer::deserialze(easy_input)};
  sudoku::solver::SudokuSolver s;

  std::vector<sudoku::datastruct::Board::matrix_value_depth> col_values;
  b.get_column(2, col_values);
  bool col_check = s.sanity_check(sudoku::utils::flatten(col_values));
  EXPECT_TRUE(col_check);
}

using Solver =sudoku::solver::SudokuSolver;
using namespace sudoku::solver;
struct SolverShould1 : ::testing::Test{
    //    0 = {1, 4, 7};
    //    1 = {1, 4};
    //    2 = {4, 5, 7};
    //    3 = {3, 5, 6};
    //    4 = {2, 3, 6, 7}; and
    //    5 = {2, 7}.
    sudoku::solver::TheBoard b;
    void SetUp(){
        b.insert({0,{1, 0, 0, 1, 0, 0, 1}});
        b.insert({1,{1, 0, 0, 1, 0, 0, 0}});
        b.insert({2,{0, 0, 0, 1, 1, 0, 1}});
        b.insert({3,{0, 0, 1, 0, 1, 1, 0}});
        b.insert({4,{0, 1, 1, 0, 0, 1, 1}});
        b.insert({5,{0, 1, 0, 0, 0, 0, 1}});
    }

};


TEST_F(SolverShould1, DetectColumns){
    Solver solver;
    ASSERT_FALSE(solver.no_columns(b));
}

TEST(SolverShould, DetectNoColumns){
    sudoku::solver::TheBoard b;

    Solver solver;
    ASSERT_TRUE(solver.no_columns(b));
}

TEST(SolverShould, GetColumnsWithLeastOnes){
    TheBoard b;
    b.insert({0, {1, 0, 0, 1, 0, 0, 1}});
    b.insert({1, {1, 0, 0, 1, 0, 0, 0}});
    b.insert({3, {0, 0, 0, 1, 0, 0, 1}});
    b.insert({4, {0, 0, 1, 0, 1, 1, 0}});
    b.insert({5, {0, 1, 1, 0, 0, 1, 1}});
    b.insert({6, {0, 1, 0, 0, 0, 0, 1}});
    auto result{get_column_with_least_ones(b)};
    std::vector<uint16_t> expected {0,0,0,1,0,0};
    size_t expected_index(4);
    ASSERT_EQ(result.first, expected);
    ASSERT_EQ(result.second, expected_index);
}
TEST_F(SolverShould1, GetColumn){

    std::vector<uint16_t> result{get_column(b,0)};
    std::vector<uint16_t> expected {1,1,0,0,0,0};
    ASSERT_EQ(result,expected);

    std::vector<uint16_t> result2{sudoku::solver::get_column(b,6)};
    std::vector<uint16_t> expected2 {1,0,1,0,1,1};
    ASSERT_EQ(result2,expected2);

    std::vector<uint16_t> result3{sudoku::solver::get_column(b,3)};
    std::vector<uint16_t> expected3 {1,1,1,0,0,0};
    ASSERT_EQ(result3,expected3);
}

TEST(SolverShould, GetIndexesOfOnes){
    TheBoard b;
    b.insert({0, {1, 0, 0, 1, 0, 0, 1}});
    b.insert({1, {1, 0, 0, 1, 0, 0, 0}});
    b.insert({3, {0, 0, 0, 1, 0, 0, 1}});
    b.insert({4, {0, 0, 1, 0, 1, 1, 0}});
    b.insert({5, {0, 1, 1, 0, 0, 1, 1}});
    b.insert({6, {0, 1, 0, 0, 0, 0, 1}});

    std::vector<size_t> ones{get_indexes_of_ones(get_column(b,0))};
    std::vector<size_t> expected{0,1};
    ASSERT_EQ(ones,expected);

    std::vector<size_t> ones2{get_indexes_of_ones(get_column(b,6))};
    std::vector<size_t> expected2{0,2,4,5};
    ASSERT_EQ(ones,expected);

    std::vector<size_t> ones3{get_indexes_of_ones(get_column(b,3))};
    std::vector<size_t> expected3{0,1,2};
    ASSERT_EQ(ones,expected);
}

TEST_F(SolverShould1, ExecuteStep6){
    Solver s;
    std::vector<size_t> indexes {0,3,6};
    TheBoard result{s.step6(indexes,b)};
//    for(auto& t : result){
//        std::cout << "first " <<t.first << "\n";
//        for(auto& el : t.second){
//            std::cout << "second " << el << "\n";
//         }
//    }
    ASSERT_EQ(1,result.size());
    std::vector<uint16_t> expected {0, 1, 1, 1};
    ASSERT_EQ(expected, result.at(3));
}

TEST_F(SolverShould1, SolveAlgoX){
    Solver s;
    TheSolution result;
    TheSolution expected{1,3,5};
    s.solve_algorithm_x(std::move(b), result);
    ASSERT_EQ(expected,result);
}

TEST(SolverShould2, SolveAlgoX){
//    Gruppe	Anton	Berta	Claudia	Daniel	Eva
//    B1	1	1	0	0	0
//    B2	1	1	1	0	0
//    B3	1	1	0	1	0
//    B4	0	0	1	0	1
//    B5	0	0	1	1	1
    TheBoard b;
    b.insert({0, {1, 1, 0, 0, 0}});
    b.insert({1, {1, 1, 1, 0, 0}});
    b.insert({3, {1, 1, 0, 1, 0}});
    b.insert({4, {0, 0, 1, 0, 1}});
    b.insert({5, {0, 0, 1, 1, 1}});

    Solver s;
    TheSolution result;
    TheSolution expected{3,4};
    s.solve_algorithm_x(std::move(b), result);
    ASSERT_EQ(expected,result);
}

TEST(CreateBoard, create_initial_board2){
    size_t board_size(2);
    size_t should_lines(board_size * board_size *board_size);

    TheBoard b = sudoku::solver::create_initial_board(board_size);
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
TEST(CreateBoard, create_initial_board3){
    size_t board_size(3);
    size_t should_lines(board_size * board_size *board_size);

    TheBoard b = sudoku::solver::create_initial_board(board_size);
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
    expected_board.insert({9, { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }});
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
}

TEST(get_box_index_first_3_should, return_values_for_4){
    auto result = sudoku::solver::get_box_index(0,4,2);
    EXPECT_EQ(0,result);

    result = sudoku::solver::get_box_index(2,4,2);
    EXPECT_EQ(0,result);

    result = sudoku::solver::get_box_index(32,4,2);
    EXPECT_EQ(2,result);

    result = sudoku::solver::get_box_index(31,4,2);
    EXPECT_EQ(1,result);

    result = sudoku::solver::get_box_index(63,4,2);
    EXPECT_EQ(3,result);
}

TEST(get_box_index_first_3_should, return_values_for_9){
    auto result = sudoku::solver::get_box_index(0,9,3);
    EXPECT_EQ(0,result);

    result = sudoku::solver::get_box_index(26,9,3);
    EXPECT_EQ(0,result);

    result = sudoku::solver::get_box_index(27,9,3);
    EXPECT_EQ(1,result);

    result = sudoku::solver::get_box_index(53,9,3);
    EXPECT_EQ(1,result);

    result = sudoku::solver::get_box_index(54,9,3);
    EXPECT_EQ(2,result);

    result = sudoku::solver::get_box_index(80,9,3);
    EXPECT_EQ(2,result);

    result = sudoku::solver::get_box_index(81,9,3);
    EXPECT_EQ(0,result);

    result = sudoku::solver::get_box_index(107,9,3);
    EXPECT_EQ(0,result);

    result = sudoku::solver::get_box_index(108,9,3);
    EXPECT_EQ(1,result);

    result = sudoku::solver::get_box_index(134,9,3);
    EXPECT_EQ(1,result);

    result = sudoku::solver::get_box_index(135,9,3);
    EXPECT_EQ(2,result);

    result = sudoku::solver::get_box_index(161,9,3);
    EXPECT_EQ(2,result);

    result = sudoku::solver::get_box_index(162,9,3);
    EXPECT_EQ(0,result);

    result = sudoku::solver::get_box_index(242,9,3);
    EXPECT_EQ(2,result);

    result = sudoku::solver::get_box_index(243,9,3);
    EXPECT_EQ(3,result);

    result = sudoku::solver::get_box_index(728,9,3);
    EXPECT_EQ(8,result);
}
