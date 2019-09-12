#include "solver.h"
#include <gtest/gtest.h>
#include "board.h"
#include "serializer.h"
#include "utils.h"

// TEST(Utils, GetRangeFrom0to2) {
//  std::vector<int> result = sudoku::utils::get_range(0, 2);
//  ASSERT_EQ(3, result.size());
//  ASSERT_EQ(0, result.at(0));
//  ASSERT_EQ(1, result.at(1));
//  ASSERT_EQ(2, result.at(2));
//}

// TEST(SolverShould, solveEasySUDOKU) {
//  std::string easy_input{"{\"0\":[[],[],[6],[4],[],[9],[5],[],[]],"
//                         "\"1\":[[],[],[3],[],[2],[],[1],[],[]],"
//                         "\"2\":[[9],[1],[],[],[7],[],[],[2],[8]],"
//                         "\"3\":[[6],[],[],[7],[],[3],[],[],[1]],"
//                         "\"4\":[[],[4],[7],[],[8],[],[3],[5],[]],"
//                         "\"5\":[[5],[],[],[2],[],[1],[],[],[9]],"
//                         "\"6\":[[4],[2],[],[],[9],[],[],[3],[6]],"
//                         "\"7\":[[],[],[1],[],[3],[],[9],[],[]],"
//                         "\"8\":[[],[],[9],[8],[],[4],[2],[],[]]}"};

//  sudoku::datastruct::Board b{::sudoku::serializer::deserialze(easy_input)};
//  sudoku::solver::SudokuSolver s;

//  std::vector<sudoku::datastruct::Board::matrix_value_depth> col_values;
//  b.get_column(2, col_values);
//  bool col_check = s.sanity_check(sudoku::utils::flatten(col_values));
//  EXPECT_TRUE(col_check);
//}

using Solver = sudoku::solver::SudokuSolver;
using namespace sudoku::solver;
// struct SolverShould1 : ::testing::Test{
//    //    0 = {1, 4, 7};
//    //    1 = {1, 4};
//    //    2 = {4, 5, 7};
//    //    3 = {3, 5, 6};
//    //    4 = {2, 3, 6, 7}; and
//    //    5 = {2, 7}.
//    sudoku::solver::TheBoard b;
//    void SetUp(){
//        b.insert({0,{1, 0, 0, 1, 0, 0, 1}});
//        b.insert({1,{1, 0, 0, 1, 0, 0, 0}});
//        b.insert({2,{0, 0, 0, 1, 1, 0, 1}});
//        b.insert({3,{0, 0, 1, 0, 1, 1, 0}});
//        b.insert({4,{0, 1, 1, 0, 0, 1, 1}});
//        b.insert({5,{0, 1, 0, 0, 0, 0, 1}});
//    }

//};

// TEST_F(SolverShould1, DetectColumns){
//    Solver solver;
//    ASSERT_FALSE(solver.no_columns(b));
//}

// TEST(SolverShould, DetectNoColumns){
//    sudoku::solver::TheBoard b;

//    Solver solver;
//    ASSERT_TRUE(solver.no_columns(b));
//}

// TEST(SolverShould, GetColumnsWithLeastOnes){
//    TheBoard b;
//    b.insert({0, {1, 0, 0, 1, 0, 0, 1}});
//    b.insert({1, {1, 0, 0, 1, 0, 0, 0}});
//    b.insert({3, {0, 0, 0, 1, 0, 0, 1}});
//    b.insert({4, {0, 0, 1, 0, 1, 1, 0}});
//    b.insert({5, {0, 1, 1, 0, 0, 1, 1}});
//    b.insert({6, {0, 1, 0, 0, 0, 0, 1}});
//    auto result{get_column_with_least_ones(b)};
//    std::vector<uint16_t> expected {0,0,0,1,0,0};
//    size_t expected_index(4);
//    ASSERT_EQ(result.first, expected);
//    ASSERT_EQ(result.second, expected_index);
//}
// TEST_F(SolverShould1, GetColumn){

//    std::vector<uint16_t> result{get_column(b,0)};
//    std::vector<uint16_t> expected {1,1,0,0,0,0};
//    ASSERT_EQ(result,expected);

//    std::vector<uint16_t> result2{sudoku::solver::get_column(b,6)};
//    std::vector<uint16_t> expected2 {1,0,1,0,1,1};
//    ASSERT_EQ(result2,expected2);

//    std::vector<uint16_t> result3{sudoku::solver::get_column(b,3)};
//    std::vector<uint16_t> expected3 {1,1,1,0,0,0};
//    ASSERT_EQ(result3,expected3);
//}

// TEST(SolverShould, GetIndexesOfOnes){
//    TheBoard b;
//    b.insert({0, {1, 0, 0, 1, 0, 0, 1}});
//    b.insert({1, {1, 0, 0, 1, 0, 0, 0}});
//    b.insert({3, {0, 0, 0, 1, 0, 0, 1}});
//    b.insert({4, {0, 0, 1, 0, 1, 1, 0}});
//    b.insert({5, {0, 1, 1, 0, 0, 1, 1}});
//    b.insert({6, {0, 1, 0, 0, 0, 0, 1}});

//    std::vector<size_t> ones{get_indexes_of_ones(get_column(b,0))};
//    std::vector<size_t> expected{0,1};
//    ASSERT_EQ(ones,expected);

//    std::vector<size_t> ones2{get_indexes_of_ones(get_column(b,6))};
//    std::vector<size_t> expected2{0,2,4,5};
//    ASSERT_EQ(ones,expected);

//    std::vector<size_t> ones3{get_indexes_of_ones(get_column(b,3))};
//    std::vector<size_t> expected3{0,1,2};
//    ASSERT_EQ(ones,expected);
//}

// TEST_F(SolverShould1, ExecuteStep6){
//    Solver s;
//    std::vector<size_t> indexes {0,3,6};
//    TheBoard result{s.step6(indexes,b)};
////    for(auto& t : result){
////        std::cout << "first " <<t.first << "\n";
////        for(auto& el : t.second){
////            std::cout << "second " << el << "\n";
////         }
////    }
//    ASSERT_EQ(1,result.size());
//    std::vector<uint16_t> expected {0, 1, 1, 1};
//    ASSERT_EQ(expected, result.at(3));
//}

// TEST_F(SolverShould1, SolveAlgoX){
//    Solver s;
//    TheSolution result;
//    TheSolution expected{1,3,5};
//    s.solve_algorithm_x(std::move(b), result);
//    ASSERT_EQ(expected,result);
//}

// TEST(SolverShould2, SolveAlgoX){
////    Gruppe	Anton	Berta	Claudia	Daniel	Eva
////    B1	1	1	0	0	0
////    B2	1	1	1	0	0
////    B3	1	1	0	1	0
////    B4	0	0	1	0	1
////    B5	0	0	1	1	1
//    TheBoard b;
//    b.insert({0, {1, 1, 0, 0, 0}});
//    b.insert({1, {1, 1, 1, 0, 0}});
//    b.insert({3, {1, 1, 0, 1, 0}});
//    b.insert({4, {0, 0, 1, 0, 1}});
//    b.insert({5, {0, 0, 1, 1, 1}});

//    Solver s;
//    TheSolution result;
//    TheSolution expected{3,4};
//    s.solve_algorithm_x(std::move(b), result);
//    ASSERT_EQ(expected,result);
//}
